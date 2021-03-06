/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CudaInPlaceImageFilter.h
  Language:  C++

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CudaInPlaceImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCudaInPlaceImageFilter_h
#define __itkCudaInPlaceImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk
{

/** \class CudaInPlaceImageFilter
 * \brief Base class for Cuda enabled filters that are able to operate
 * inplace.
 * 
 * This is a modification of the standard InPlaceImageFilter for Cuda
 * enabled filters.
 *
 * \author Phillip Ward, Luke Parkinson, Daniel Micevski, Christopher
 * Share, Victorian Partnership for Advanced Computing (VPAC). 
 * Richard Beare, Monash University
 * \ingroup ImageFilters
 */
template <class TInputImage, class TOutputImage=TInputImage>
class ITK_EXPORT CudaInPlaceImageFilter : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef CudaInPlaceImageFilter                             Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage>  Superclass;
  typedef SmartPointer<Self>                             Pointer;
  typedef SmartPointer<const Self>                       ConstPointer;


  /** Run-time type information (and related methods). */
  itkTypeMacro(CudaInPlaceImageFilter,ImageToImageFilter);

  /** Superclass typedefs. */
  typedef typename Superclass::OutputImageType       OutputImageType;
  typedef typename Superclass::OutputImagePointer    OutputImagePointer;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType  OutputImagePixelType;

  /** Some convenient typedefs. */
  typedef TInputImage                             InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointer;
  typedef typename InputImageType::ConstPointer   InputImageConstPointer;
  typedef typename InputImageType::RegionType     InputImageRegionType;
  typedef typename InputImageType::PixelType      InputImagePixelType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** In place operation can be turned on and off. Asking for
   * in-place operation, i.e. calling SetInplace(true) or InplaceOn(),
   * will be effective only if CanRunInPlace also returns true.
   * By default CanRunInPlace checks whether the input and output
   * image type match. */
  itkSetMacro(InPlace, bool);
  itkGetConstMacro(InPlace, bool);
  itkBooleanMacro(InPlace);

  /** Can the filter run in place? To do so, the filter's first input
   * and output must have the same dimension and pixel type. This
   * method can be used in conjunction with the InPlace ivar to
   * determine whether a particular use of the filter is really
   * running in place. Some filters may be able to optimize their
   * operation if the InPlace is true and CanRunInPlace is true.
   * CanRunInPlace may also be overridded by CudaInPlaceImageFilter
   * subclasses to fine tune its behavior. */
  virtual bool CanRunInPlace() const
    {
    return (typeid(TInputImage) == typeid(TOutputImage));
    }

protected:
  CudaInPlaceImageFilter();
  ~CudaInPlaceImageFilter();

  virtual void PrintSelf(std::ostream& os, Indent indent) const;

  /** The GenerateData method normally allocates the buffers for all
   * of the outputs of a filter. Since CudaInPlaceImageFilter's can use an
   * overwritten version of the input for its output, the output
   * buffer should not be allocated. When possible, we graft the input
   * to the filter to the output.  If an InPlaceFilter has multiple
   * outputs, then it would need to override this method to graft one
   * of its outputs and allocate the remaining. Cuda filters will need
   * to call this explicitly from inside GenerateData. */
  virtual void AllocateOutputs();

  /** CudaInPlaceImageFilter may transfer ownership of the input bulk data
   * to the output object.  Once the output object owns the bulk data
   * (done in AllocateOutputs()), the input object must release its
   * hold on the bulk data.  ProcessObject::ReleaseInputs() only
   * releases the input bulk data when the user has set the
   * ReleaseDataFlag.  CudaInPlaceImageFilter::ReleaseInputs() also
   * releases the input that it has overwritten.
   *
   * \sa ProcessObject::ReleaseInputs() */
  virtual void ReleaseInputs();

private:
  CudaInPlaceImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_InPlace;

};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_CudaInPlaceImageFilter(_, EXPORT, x, y) namespace itk { \
  _(2(class EXPORT CudaInPlaceImageFilter< ITK_TEMPLATE_2 x >)) \
  namespace Templates { typedef CudaInPlaceImageFilter< ITK_TEMPLATE_2 x > CudaInPlaceImageFilter##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/CudaInPlaceImageFilter+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "CudaInPlaceImageFilter.txx"
#endif

#endif
