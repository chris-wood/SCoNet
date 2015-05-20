/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2015 University of California, Santa Cruz
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: James Mathewson <jlmathew@soe.ucsc.edu>
 *
 *
 */
 #include <iostream>

#ifndef _ICN_RANGEDATA_T_
#define _ICN_RANGEDATA_T_

template < class T >
class RangeData;

template < class T >
std::ostream & operator << (std::ostream &, const RangeData<T> &);

template <class T >
 class RangeData
 {
    public:
      RangeData();
      RangeData(T const &value);
      RangeData(T const &lower, T const &upper);
      RangeData(const RangeData<T> &range);
      virtual ~RangeData();

      void setLowerBound(const T &rhs);
      void setUpperBound(const T &rhs);
      T getLowerBound();
      T getUpperBound();
      bool isInRange(T &other) const;
      bool isExactMatch(const RangeData<T> &range) const;

      T & operator=(const T &rhs);
      bool operator==(const RangeData<T> &rhs) const;
      bool operator!=(const RangeData<T> &rhs) const;
      bool operator<(const RangeData<T> &rhs) const;

      // FRIEND FUNCTIONS
    friend std::ostream & operator << <>(std::ostream &os, const RangeData<T> &rhs);

   private:
      T lowerBound, upperBound;

 };

template < class T >
std::ostream & operator << (std::ostream & out, const RangeData<T> & rhs){
    out <<  "(" << rhs.lowerBound << "," << rhs.upperBound << ")";
    return (out);
}

template <class T>
RangeData<T>::RangeData(T const &_lowerBound, T const &_higherBound)
{
   lowerBound = _lowerBound;
   upperBound = _higherBound;

}
template <class T>
RangeData<T>::RangeData()
{

}
template <class T>
RangeData<T>::RangeData(T const &_number)
{
    lowerBound = _number;
    upperBound = _number;

}
template <class T>
RangeData<T>::RangeData(const RangeData<T> &_rangeData)
{
    lowerBound = _rangeData.lowerBound;
    upperBound = _rangeData.upperBound;

}

template <class T>
RangeData<T>::~RangeData()
{
}

template <class T>
void RangeData<T>::setLowerBound(T const &_lowerBound)
{
  lowerBound = _lowerBound;
}
template <class T>
void RangeData<T>::setUpperBound(T const &_upperBound)
{
  upperBound = _upperBound;
}
template <class T>
T RangeData<T>::getLowerBound()
{
  return lowerBound;
}
template <class T>
T RangeData<T>::getUpperBound()
{
  return upperBound;
}
template <class T>
bool RangeData<T>::isInRange(T &other) const
{
  if ((other >= lowerBound) && (other <= upperBound))
    return true;
  return false;
}
template <class T>
bool RangeData<T>::isExactMatch(const RangeData<T> &range) const
{
  if ((range.getLowerBound == lowerBound) && (range.getUpperBound == upperBound))
    return true;
  return false;
}

template <class T>
T& RangeData<T>::operator=(const T &rhs) 
{
   // Check for self-assignment!
    if (this == &rhs)      // Same object?
      return *this;        // Yes, so skip assignment, and just return *this.
    
    lowerBound = rhs.getLowerBound();
    upperBound = rhs.getUpperBound();
    return *this;
}

template <class T>
bool RangeData<T>::operator!=(const RangeData<T> &rhs) const
{
   return !((lowerBound == rhs.lowerBound) && (upperBound == rhs.upperBound));
}


template <class T>
bool RangeData<T>::operator==(const RangeData<T> &rhs) const
{
    
   return (lowerBound == rhs.lowerBound) && (upperBound == rhs.upperBound);
}

template <class T>
bool RangeData<T>::operator<(const RangeData<T> &rhs) const
{
  if ((rhs.lowerBound < lowerBound))
    return true;
  return false;
 }

#endif
