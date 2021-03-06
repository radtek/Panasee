/*=========================================================================

  Program:   ParaView
  Module:    $RCSfile: vtkXMLMultiGroupDataReader.h,v $

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkXMLMultiGroupDataReader - Reader for multi-block datasets
// .SECTION Description
// vtkXMLMultiGroupDataReader is a legacy reader that reads multi group files
// into multiblock datasets.

#ifndef __vtkXMLMultiGroupDataReader_h
#define __vtkXMLMultiGroupDataReader_h

#include "vtkXMLMultiBlockDataReader.h"

class VTK_IO_EXPORT vtkXMLMultiGroupDataReader : public vtkXMLMultiBlockDataReader
{
public:
  static vtkXMLMultiGroupDataReader* New();
  vtkTypeRevisionMacro(vtkXMLMultiGroupDataReader,vtkXMLMultiBlockDataReader);
  void PrintSelf(ostream& os, vtkIndent indent);

protected:
  vtkXMLMultiGroupDataReader();
  ~vtkXMLMultiGroupDataReader();  

  // Get the name of the data set being read.
  virtual const char* GetDataSetName()
    {
    return "vtkMultiGroupDataSet";
    }

private:
  vtkXMLMultiGroupDataReader(const vtkXMLMultiGroupDataReader&);  // Not implemented.
  void operator=(const vtkXMLMultiGroupDataReader&);  // Not implemented.
};

#endif
