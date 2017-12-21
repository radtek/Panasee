
#ifndef __MeshWriter_h
#define __MeshWriter_h

#include "vtkPolyDataAlgorithm.h"

class CMeshWriter : public vtkPolyDataAlgorithm 
{
public:
  static CMeshWriter *New();
  vtkTypeRevisionMacro(CMeshWriter,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Specify file name of Wavefront .obj file.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

  int Writer(vtkPolyData *input);
  int Writer(int NumberOfMeshPoints,float *MeshPoints,
    int NumberOfMeshCells,int *MeshCells);
protected:
  CMeshWriter();
  ~CMeshWriter();
  
  

  char *FileName;
private:
  CMeshWriter(const CMeshWriter&);  // Not implemented.
  void operator=(const CMeshWriter&);  // Not implemented.
};

#endif
