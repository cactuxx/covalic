
#include "DiceOverlapImageToImageMetric.h"
#include "JaccardOverlapImageToImageMetric.h"
//#include "CohenKappaImageToImageMetric.h"

#include "itkImage.h"
#include "itkImageRegionIteratorWithIndex.h"

#include "itkOutputWindow.h"
#include "itkTextOutput.h"

#include <exception>
#include <iostream>
#include <string>


int
testMetrics()
{

  itk::OutputWindow::SetInstance(itk::TextOutput::New());

  typedef itk::Image<unsigned char, 3> ByteImageType;

  ByteImageType::SizeType size = {{64, 64, 64}};

  ByteImageType::RegionType region;
  region.SetSize(size);

  // Create fake images
  ByteImageType::Pointer Amask = ByteImageType::New();
  Amask->SetRegions(region);
  Amask->Allocate();
  Amask->FillBuffer(0);

  ByteImageType::Pointer Bmask = ByteImageType::New();
  Bmask->SetRegions(region);
  Bmask->Allocate();
  Bmask->FillBuffer(0);

  ByteImageType::IndexType ind;
  for (ind[2] = 0; ind[2] < (long)size[2]; ind[2]++)
    for (ind[1] = 0; ind[1] < (long)size[1]; ind[1]++)
      for (ind[0] = 0; ind[0] < (long)size[0]; ind[0]++)
      {
        if (ind[2] < 16 || ind[2] > 48)
          continue;
        if (ind[1] < 16 || ind[1] > 48)
          continue;
        if (ind[0] < 16 || ind[0] > 48)
          continue;
        Amask->SetPixel(ind, 1);

        if (ind[0] < 32)
          continue;
        Bmask->SetPixel(ind, 1);
      }


  typedef DiceOverlapImageToImageMetric<ByteImageType, ByteImageType>
    DiceMetricType;
  DiceMetricType::Pointer diceMetric = DiceMetricType::New();
  diceMetric->SetFixedImage(Amask);
  diceMetric->SetMovingImage(Bmask);

  std::cout << "Dice(A,B) = " <<  diceMetric->GetValue() << std::endl;

  typedef JaccardOverlapImageToImageMetric<ByteImageType, ByteImageType>
    JaccardMetricType;
  JaccardMetricType::Pointer jaccMetric = JaccardMetricType::New();
  jaccMetric->SetFixedImage(Amask);
  jaccMetric->SetMovingImage(Bmask);

  std::cout << "Jaccard(A,B) = " <<  jaccMetric->GetValue() << std::endl;

  return 0;

}

int
main(int argc, char** argv)
{
  try
  {
    testMetrics();
  } 
  catch (itk::ExceptionObject& e)
  {
    std::cerr << e << std::endl;
    return -1;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
    return -1;
  }
  catch (std::string& s)
  {
    std::cerr << "Exception: " << s << std::endl;
    return -1;
  }
  catch (...)
  {
    std::cerr << "Unknown exception" << std::endl;
    return -1;
  }

  return 0;


}
