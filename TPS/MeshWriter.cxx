/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkOBJReader.cxx,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "MeshWriter.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"

vtkCxxRevisionMacro(CMeshWriter, "$Revision: 1.27 $");
vtkStandardNewMacro(CMeshWriter);

// Description:
// Instantiate object with NULL filename.
CMeshWriter::CMeshWriter()
{
  this->FileName = NULL;

  this->SetNumberOfInputPorts(0);
}

CMeshWriter::~CMeshWriter()
{
  if (this->FileName)
    {
    delete [] this->FileName;
    this->FileName = NULL;
    }
}

int CMeshWriter::Writer(int NumberOfMeshPoints,float *MeshPoints,
    int NumberOfMeshCells,int *MeshCells)
{
	if(MeshPoints==NULL || MeshCells==NULL || 
		NumberOfMeshPoints<=3 || NumberOfMeshCells<=3 )
	{
	  return 0;
	}

  if (!this->FileName) 
    {
    vtkErrorMacro(<< "A FileName must be specified.");
    return 0;
    }
    
  FILE *in = fopen(this->FileName,"w+");
    
  if (in == NULL) 
    {
    vtkErrorMacro(<< "File " << this->FileName << " not found");
    return 0;
    }

  vtkDebugMacro(<<"Writing file");



  const int MAX_LINE=1024;
  char line[MAX_LINE];
  double xyz[3];

  int iPntCount=NumberOfMeshPoints;
  for(int i=0;i<iPntCount;i++)
  {
	  xyz[0]=MeshPoints[i*3];
	  xyz[1]=MeshPoints[i*3+1];
	  xyz[2]=MeshPoints[i*3+2];
	  sprintf(line,"v %.6f %.6f %.6f\n",xyz[0],xyz[1],xyz[2]);
	  fputs(line,in);
  }

  int iCount=NumberOfMeshCells;
  int *pData=MeshCells;
  int iCellPnts;
  for(i=0;i<iCount;i++)
  {
	  iCellPnts=*pData;
	  if(iCellPnts==3)
	  {
		sprintf(line,"f %i %i %i\n",pData[1]+1,pData[2]+1,pData[3]+1);
		fputs(line,in);
	  }

	  if(iCellPnts==4)
	  {
		  sprintf(line,"f %i %i %i\n",pData[1]+1,pData[2]+1,pData[3]+1);
		  fputs(line,in);
		  sprintf(line,"f %i %i %i\n",pData[3]+1,pData[4]+1,pData[1]+1);
		  fputs(line,in);
	  }
	  pData+=iCellPnts+1;

  }

  fclose(in);

  return 1;
}

int CMeshWriter::Writer(vtkPolyData *input)
{
  // get the info object
  if(input==NULL)
	  return 0;

  if (!this->FileName) 
    {
    vtkErrorMacro(<< "A FileName must be specified.");
    return 0;
    }
    
  FILE *in = fopen(this->FileName,"w+");
    
  if (in == NULL) 
    {
    vtkErrorMacro(<< "File " << this->FileName << " not found");
    return 0;
    }

  vtkDebugMacro(<<"Writing file");


  vtkPoints *points=input->GetPoints();
  vtkCellArray *polys=input->GetPolys(); //GetStrips();

  if(points==NULL || polys==NULL)
	  return 0;

  const int MAX_LINE=1024;
  char line[MAX_LINE];
  double xyz[3];
  int n_pts=-1,*pts=NULL;

  int iPntCount=points->GetNumberOfPoints();
  for(int i=0;i<iPntCount;i++)
  {
	  points->GetPoint(i,xyz);
	  sprintf(line,"v %.6f %.6f %.6f\n",xyz[0],xyz[1],xyz[2]);
	  fputs(line,in);
  }

  int iCount=polys->GetNumberOfCells();
  polys->InitTraversal();
  for(i=0;i<iCount;i++)
  {
	  polys->GetNextCell(n_pts,pts);
	  if(n_pts==3)
	  {
		  sprintf(line,"f %i %i %i\n",pts[0]+1,pts[1]+1,pts[2]+1);
		  fputs(line,in);
	  }

	  if(n_pts==4)
	  {
		  sprintf(line,"f %i %i %i %i\n",pts[0]+1,pts[1]+1,pts[2]+1,pts[3]+1);
		  //sprintf(line,"f %i %i %i \n",pts[0],pts[1],pts[2]);
		  //fputs(line,in);
		  //sprintf(line,"f %i %i %i \n",pts[2],pts[3],pts[0]);
		  //fputs(line,in);
	  }
  }

  fclose(in);

  return 1;

}

void CMeshWriter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "File Name: " 
     << (this->FileName ? this->FileName : "(none)") << "\n";

}
