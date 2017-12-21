
#ifndef PrintParameters_h
#define PrintParameters_h 1
#include "dicom.hpp"
#include "DICOMEnums.h"
#include <string>
using namespace std;
class ImageBox 
{

  public:
	  ImageBox()
	  {
		  ImagePosition=0;
		  SamplesPerPixel=0;
		  PlanarConfiguration=0;
		  Rows=0;
		  Columns=0;
		  BitsAllocated=0;
		  BitsStored=0;
		  HighBit=0;
		  PixelRepresentation=0;
		  PixelData=NULL;
		  WinWidth=0;
		  WinCenter=0;
		  RescaleIntercept=0;
		  RescaleSlope=0;
	  }
      unsigned short ImagePosition;
      unsigned short SamplesPerPixel;
      unsigned short PlanarConfiguration;
      unsigned short Rows;
      unsigned short Columns;
      string PixeAspectRatio;
	  string PhotometricInterpretation;
      unsigned short BitsAllocated;
      unsigned short BitsStored;
      unsigned short HighBit;
      unsigned short PixelRepresentation;
      BYTE *PixelData;
      DICOM_PRINT_POLARITY Polarity;
	  int WinWidth;
	  int WinCenter;
	  float RescaleIntercept;
	  float RescaleSlope;
};

class FilmBox 
{

  public:
	  FilmBox()
	  {
		  m_ImageBox.Clear();
	  }
      string ImageDisplayFormat;
      DICOM_PRINT_FILMORIENTATION FilmOrientation;
      DICOM_PRINT_FILMSIZEID FilmSizeID;
      DICOM_PRINT_MAGNIFICATIONTYPE MagnificationType;
      UID SOPInstanceUID;
      Array<ImageBox> m_ImageBox;
};

class FilmSession 
{

  public:
	  FilmSession()
	  {
		  m_FilmBox.Clear();
	  }

      int NumberofCopies;
      DICOM_PRINT_PRINTPRIORITY PrintPriority;
      DICOM_PRINT_MEDIUMTYPE MediumType;
      DICOM_PRINT_FILMDESTINATION FilmDestination;
      UID SOPInstanceUID;
      Array<FilmBox *> m_FilmBox;
};

#endif
