/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkTypedArray.h.in,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkTypeInt64Array - dynamic, self-adjusting array of vtkTypeInt64
// .SECTION Description
// vtkTypeInt64Array is an array of values of type vtkTypeInt64.  It
// provides methods for insertion and retrieval of values and will
// automatically resize itself to hold new data.

#ifndef __vtkTypeInt64Array_h
#define __vtkTypeInt64Array_h

#include "vtk__Int64Array.h"

class VTK_COMMON_EXPORT vtkTypeInt64Array : public vtk__Int64Array
{
public:
  static vtkTypeInt64Array* New();
  vtkTypeRevisionMacro(vtkTypeInt64Array,vtk__Int64Array);
  void PrintSelf(ostream& os, vtkIndent indent);

protected:
  vtkTypeInt64Array(vtkIdType numComp=1);
  ~vtkTypeInt64Array();

private:
  vtkTypeInt64Array(const vtkTypeInt64Array&);  // Not implemented.
  void operator=(const vtkTypeInt64Array&);  // Not implemented.
};

#endif
