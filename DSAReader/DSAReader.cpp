#define DLL_EXPORTS
#include "DSAReader.h"

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmjpeg/djencode.h>
#include <dcmtk/ofstd/ofcond.h>

inline float ofstr_to_float(OFString &str) {
	return static_cast<Float32>(atof((const char *)str.c_str()));
}

inline float *ofstr_to_float_array(OFString &str, int count) {
	Float32 *l_ret_vec = new Float32[count];
	int l_ret_vec_top = 0;

	const size_t l_str_length = str.length();
	size_t l_idx = 0;
	size_t l_last_idx = 0;
	while (l_idx < l_str_length) {
		while (l_idx < l_str_length && str.at(l_idx) != '\\')
			++l_idx;
		if (str.at(l_idx - 1) != '\\') {
			OFString l_tmp_str = str.substr(l_last_idx, l_idx - l_last_idx);
			l_ret_vec[l_ret_vec_top++] = ofstr_to_float(l_tmp_str);
		}
		l_last_idx = l_idx + 1;
		l_idx = l_last_idx;
	}
	return l_ret_vec;
}

DsaData::DsaData(std::string dsa_path) {
	dsa = LoadDcmDataSet(dsa_path);
}

DsaData::~DsaData() {
}

std::vector<cv::Mat> DsaData::LoadDcmDataSet(std::string file_path) {
	DcmFileFormat fileformat;
	OFCondition oc = fileformat.loadFile(file_path.c_str());
	if (!oc.good()) {
		std::cout << "file Load error" << std::endl;
		std::vector<cv::Mat> emptyVec;
		return emptyVec;
	}
	DcmDataset *dataset = fileformat.getDataset();
	E_TransferSyntax xfer = dataset->getOriginalXfer();

	OFString patientname;
	dataset->findAndGetOFString(DCM_PatientName, patientname);

	unsigned short bit_count(0);
	dataset->findAndGetUint16(DCM_BitsStored, bit_count);

	OFString isRGB;
	dataset->findAndGetOFString(DCM_PhotometricInterpretation, isRGB);

	unsigned short img_bits(0);
	dataset->findAndGetUint16(DCM_SamplesPerPixel, img_bits);
	int Img_bitCount = (int)img_bits;

	OFString framecount;
	dataset->findAndGetOFString(DCM_NumberOfFrames, framecount);

	unsigned short m_width;
	unsigned short m_height;
	dataset->findAndGetUint16(DCM_Rows, m_height);
	dataset->findAndGetUint16(DCM_Columns, m_width);
	
	OFString dis_detector_str, dis_patient_str;
	dataset->findAndGetOFString(DCM_DistanceSourceToDetector, dis_detector_str);
	dataset->findAndGetOFString(DCM_DistanceSourceToPatient, dis_patient_str);
	distance_source_detector = ofstr_to_float(dis_detector_str);
	distance_source_patient = ofstr_to_float(dis_patient_str);

	OFString pixel_spacing_str;
	OFCondition l_status = dataset->findAndGetOFStringArray(DCM_PixelSpacing, pixel_spacing_str, true);
	if (!l_status.good())
		dataset->findAndGetOFStringArray(DCM_ImagerPixelSpacing, pixel_spacing_str, true);
	Float32 *l_spacing = ofstr_to_float_array(pixel_spacing_str, 2);
	pixel_spacing[0] = l_spacing[0];
	pixel_spacing[1] = l_spacing[1];

	const char* transferSyntax = NULL;
	fileformat.getMetaInfo()->findAndGetString(DCM_TransferSyntaxUID, transferSyntax);
	std::string losslessTransUID = "1.2.840.10008.1.2.4.70";
	std::string lossTransUID = "1.2.840.10008.1.2.4.51";
	std::string losslessP14 = "1.2.840.10008.1.2.4.57";
	std::string lossyP1 = "1.2.840.10008.1.2.4.50";
	std::string lossyRLE = "1.2.840.10008.1.2.5";
	if (transferSyntax == losslessTransUID || transferSyntax == lossTransUID ||
		transferSyntax == losslessP14 || transferSyntax == lossyP1) {
		DJDecoderRegistration::registerCodecs();
		dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
		DJDecoderRegistration::cleanup();
	}
	else if (transferSyntax == lossyRLE) {
		DcmRLEDecoderRegistration::registerCodecs();
		dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
		DcmRLEDecoderRegistration::cleanup();
	}
	else {
		dataset->chooseRepresentation(xfer, NULL);
	}

	DicomImage* m_dcmImage = new DicomImage((DcmObject*)dataset, xfer);

	std::vector<cv::Mat> output_img;
	this->frame_num = m_dcmImage->getFrameCount();
	for (int k = 0; k < this->frame_num; k++) {
		unsigned char *pixelData = (unsigned char*)(m_dcmImage->getOutputData(8, k, 0));
		if (pixelData != NULL) {
			int m_height = m_dcmImage->getHeight();
			int m_width = m_dcmImage->getWidth();
			if (3 == Img_bitCount) {
				cv::Mat dst2(m_height, m_width, CV_8UC3, cv::Scalar::all(0));
				for (int i = 0; i < m_height; i++) {
					for (int j = 0; j < m_width; j++) {
						dst2.at<cv::Vec3b>(i, j)[0] = *(pixelData + i * m_width * 3 + j * 3 + 2);
						dst2.at<cv::Vec3b>(i, j)[1] = *(pixelData + i * m_width * 3 + j * 3 + 1);
						dst2.at<cv::Vec3b>(i, j)[2] = *(pixelData + i * m_width * 3 + j * 3);
					}
				}
				output_img.push_back(dst2);
			}
			else if (1 == Img_bitCount) {
				cv::Mat dst2(m_height, m_width, CV_8UC1, cv::Scalar::all(0));
				uchar* data = nullptr;
				for (int i = 0; i < m_height; i++) {
					data = dst2.ptr<uchar>(i);
					for (int j = 0; j < m_width; j++) {
						data[j] = *(pixelData + i * m_width + j);
					}
				}
				output_img.push_back(dst2);
			}
		}
	}
	return output_img;
}
