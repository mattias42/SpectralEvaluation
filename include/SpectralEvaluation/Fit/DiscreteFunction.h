/**
* Contains a discrete function.
*
* @author		\URL[Stefan Kraus]{http://stefan@00kraus.de} @ \URL[IWR, Image Processing Group]{http://klimt.iwr.uni-heidelberg.de}
* @version		1.0 @ 2002/02/21
*/
#if !defined(DISCRETEFUNCTION_H_020221)
#define DISCRETEFUNCTION_H_020221

#include "ParamFunction.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MSC_VER
#pragma warning (push, 3)
#endif

namespace MathFit
{
	/**
	* This object is a function that consists of sampled data values.
	* Each data sample starts at its appropriate X value and keeps the same value until 
	* the next data sample starts. No interpolation will be done. The functions it self behaves
	* like a discredization function:
	\begin{verbatim}
	f(x) = mYData[x_i]
	where {em x_i} is defined by
	x is element of [x_i:x_i+1]
	x_i is element of mXData[x_0:x_n-1]
	\end{verbatim}
	*
	* {\bf NOTE:} The X values must be sorted in ascending order!
	*
	* @author		\URL[Stefan Kraus]{http://stefan@00kraus.de} @ \URL[IWR, Image Processing Group]{http://klimt.iwr.uni-heidelberg.de}
	* @version		1.0 @ 2001/02/21
	*/
	class CDiscreteFunction : public IParamFunction
	{
	public:
		/**
		* Create an empty object.
		*/
		CDiscreteFunction()
		{
		}

		/**
		* Create an object using the given data.
		*
		* @param vXValues			The vector containing the X values.
		* @param vYValues			The vector containing the Y values in regard to the X values.
		*
		* @exception	CBSplineFailed
		*/
		CDiscreteFunction(CVector& vXValues, CVector& vYValues)
		{
			SetData(vXValues, vYValues);
		}

		/**
		* Create an object using the given data.
		*
		* @param vXValues			The vector containing the X values.
		* @param vYValues			The vector containing the Y values in regard to the X values.
		* @param vError			The vector containing the errors of the Y values. This vector will not be interpolated!
		*
		* @exception	CBSplineFailed
		*/
		CDiscreteFunction(CVector& vXValues, CVector& vYValues, CVector& vError)
		{
			SetData(vXValues, vYValues, vError);
		}

		/**
		* Copies the given data into the object. 
		* The error is set to one.
		*
		* @param vXValues		A vector object containing the X values of the data set.
		* @param vYValues		A vector object containing the Y values of the data set.
		*
		* @return TRUE is successful, false if the vector sizes do not match.
		*/
		virtual bool SetData(CVector& vXValues, CVector& vYValues)
		{
			// create a neutral error vector
			CVector vError(vXValues.GetSize());
			vError.Wedge(1, 0);

			return SetData(vXValues, vYValues, vError);
		}

		/**
		* Sets the new function values.
		*
		* @param vXValues		A vector object containing the X values of the data set.
		* @param vYValues		A vector object containing the Y values of the data set.
		* @param vError			A vector object containing the <B>sigma</B> error values of the data set.
		*
		* @return TRUE is successful, false if the vector sizes do not match.
		*
		* @see	IDataPoints::SetData
		*/
		virtual bool SetData(CVector& vXValues, CVector& vYValues, CVector& vError)
		{
			// first copy data into internal buffers
			if(!IParamFunction::SetData(vXValues, vYValues, vError))
				return false;

			if(!MakeDiscreteSlopes())
				return false;
			return true;
		}

		/**
		* Returns the value of the function at the given X value.
		*
		* @param fXValue	The X value at which to evaluate the B-Spline
		*
		* @return	The data sample value at the given data point.
		*/
		virtual TFitData GetValue(TFitData fXValue)
		{
			// find appropriate interval in the X values
			int iIndex = FindXIndex(fXValue);
			return mYData.GetAt(iIndex);
		}

		/**
		* Calculates the function values at a set of given data points.
		* {\bf NOTE:} The X values must be sorted in ascending order!
		*
		* @param vXValues			A vector object containing the X values at which the function has to be evaluated.
		* @param vYTargetVector	A vector object which receives the resulting function values.
		*
		* @return	A reference to the Y vector object
		*/
		virtual CVector& GetValues(CVector& vXValues, CVector& vYTargetVector)
		{
			int iIndex = FindXIndex(vXValues.GetAt(0));
			TFitData fNextXData = mXData.GetAtSafe(iIndex + 1);
			const int iXSize = vXValues.GetSize();
			const int iXSizeBase = mXData.GetSize();
			int i;
			for(i = 0; i < iXSize; i++)
			{
				while(vXValues.GetAt(i) >= fNextXData && iIndex < iXSizeBase)
				{
					iIndex++;
					fNextXData = mXData.GetAtSafe(iIndex + 1);
				}
				vYTargetVector.SetAt(i, mYData.GetAtSafe(iIndex));
			}

			return vYTargetVector;
		}

		/**
		* Returns the first derivative of the B-Spline at the given data point.
		*
		* @param fXValue	The X value at which the slope is needed.
		*
		* @return	The slope of the B-Spline at the given data point.
		*/
		virtual TFitData GetSlope(TFitData fXValue)
		{
			// find appropriate interval in the X values
			int iIndex = FindXIndex(fXValue);
			return mDiscreteSlopes.GetAt(iIndex);
		}

		/**
		* Calculates the first derivative of the function at a set of given data points.
		*
		* @param vXValues		A vector object containing the X values at which the function has to be evaluated.
		* @param vSlopeVector	A vector object which receives the resulting function values.
		*
		* @return	A reference to the slope vector object.
		*/
		virtual CVector& GetSlopes(CVector& vXValues, CVector& vSlopeVector)
		{
			int iIndex = FindXIndex(vXValues.GetAt(0));
			TFitData fNextXData = mXData.GetAtSafe(iIndex + 1);
			const int iXSize = vXValues.GetSize();
			const int iXSizeBase = mXData.GetSize();
			int i;
			for(i = 0; i < iXSize; i++)
			{
				while(vXValues.GetAt(i) >= fNextXData && iIndex < iXSizeBase)
				{
					iIndex++;
					fNextXData = mXData.GetAtSafe(iIndex + 1);
				}
				vSlopeVector.SetAt(i, mDiscreteSlopes.GetAtSafe(iIndex));
			}

			return vSlopeVector;
		}

		/**
		* Returns the basis function of the specified linear parameter.
		* A basis function is defined as the term by which the linear parameter is multiplied.
		*
		* @param fXValue	The data point at which the basis function should be determined.
		* @param iParamID	The index within the linear parameter vector of the linear parameter.
		* @param bFixedID	If TRUE the given parameter ID is the parameter ID without all fixed parameter.
		*
		* @return	The basis function in regard to the given linear parameter.
		*/
		virtual TFitData GetLinearBasisFunction(TFitData /*fXValue*/, int /*iParamID*/, bool /*bFixedID = true*/)
		{
			throw(EXCEPTION(CNotImplementedException));
		}

	private:
		/**
		* Finds the nearest index within the X data vector that matches the given data point.
		* As best fit the highest X data vector element that is less or equal the given data point
		* will be used.
		* This method works recursively using an interval halfing algorithm.
		*
		* @param fXValue	The data point which's index is needed.
		* @param iLower	The current lower index of the interval to be searched.
		* @param iUpper	The current upper index of the interval to be searched.
		*
		* @return	The index within the X data v�ctor that matches best the given data point.
		*/
		int FindXIndex(TFitData fXValue, int iLower = 0, int iUpper = -1) const
		{
			if(iUpper < 0)
				iUpper = mXData.GetSize() - 1;

			if((iUpper - iLower) <= 1)
				return iLower;

			int iMid = iLower + (iUpper - iLower) / 2;
			if(fXValue >= mXData.GetAt(iMid))
				return FindXIndex(fXValue, iMid, iUpper);
			else
				return FindXIndex(fXValue, iLower, iMid);
		}

		/**
		* Precalculate the slope using a Sobel filter.
		*/
		bool MakeDiscreteSlopes()
		{
			const int iMaxIndex = mXData.GetSize() - 1;

			mDiscreteSlopes.SetSize(iMaxIndex + 1);

			int i;
			for(i = 1; i < iMaxIndex; i++)
				mDiscreteSlopes.SetAt(i, (mYData.GetAt(i + 1) - mYData.GetAt(i - 1)) / (mXData.GetAt(i + 1) - mXData.GetAt(i - 1)));

			mDiscreteSlopes.SetAt(0, (mYData.GetAt(1) - mYData.GetAt(0)) / (mXData.GetAt(1) - mXData.GetAt(0)));
			mDiscreteSlopes.SetAt(iMaxIndex, (mYData.GetAt(iMaxIndex) - mYData.GetAt(iMaxIndex - 1)) / (mXData.GetAt(iMaxIndex) - mXData.GetAt(iMaxIndex - 1)));

			return true;
		}

		CVector mDiscreteSlopes;
	};
}

#ifdef _MSC_VER
#pragma warning (pop)
#endif

#endif
