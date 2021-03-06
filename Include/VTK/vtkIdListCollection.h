/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkIdListCollection.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkIdListCollection - maintain an unordered list of dataarray objects
// .SECTION Description
// vtkIdListCollection is an object that creates and manipulates lists of
// datasets. See also vtkCollection and subclasses.

#ifndef __vtkIdListCollection_h
#define __vtkIdListCollection_h

#include "vtkCollection.h"

#include "vtkIdList.h" // Needed for inline methods

class VTK_COMMON_EXPORT vtkIdListCollection : public vtkCollection
{
public:
  static vtkIdListCollection *New();
  vtkTypeRevisionMacro(vtkIdListCollection,vtkCollection);

  // Description:
  // Add a dataset to the list.
  void AddItem(vtkIdList *ds)
    {
      this->vtkCollection::AddItem(ds);
    }
  
  // Description:
  // Get the next dataset in the list.
  vtkIdList *GetNextItem() { 
    return static_cast<vtkIdList *>(this->GetNextItemAsObject());};

  // Description:
  // Get the ith dataset in the list.
  vtkIdList *GetItem(int i) { 
    return static_cast<vtkIdList *>(this->GetItemAsObject(i));};

  //BTX
  // Description: 
  // Reentrant safe way to get an object in a collection. Just pass the
  // same cookie back and forth. 
  vtkIdList *GetNextIdList(vtkCollectionSimpleIterator &cookie) {
    return static_cast<vtkIdList *>(this->GetNextItemAsObject(cookie));};
  //ETX
  
protected:
  vtkIdListCollection() {};
  ~vtkIdListCollection() {};


private:
  // hide the standard AddItem from the user and the compiler.
  void AddItem(vtkObject *o) { this->vtkCollection::AddItem(o); };

private:
  vtkIdListCollection(const vtkIdListCollection&);  // Not implemented.
  void operator=(const vtkIdListCollection&);  // Not implemented.
};


#endif
