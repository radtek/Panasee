/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkFollower.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkFollower - a subclass of actor that always faces the camera
// .SECTION Description
// vtkFollower is a subclass of vtkActor that always follows its specified 
// camera. More specifically it will not change its position or scale,
// but it will continually update its orientation so that it is right side
// up and facing the camera. This is typically used for text labels in a
// scene. All of the adjustments that can be made to an actor also will
// take effect with a follower.  So, if you change the orientation of the
// follower by 90 degrees, then it will follow the camera, but be off by 
// 90 degrees.

// .SECTION see also
// vtkActor vtkCamera

#ifndef __vtkFollower_h
#define __vtkFollower_h

#include "vtkActor.h"

class vtkCamera;

class VTK_RENDERING_EXPORT vtkFollower : public vtkActor
{
 public:
  vtkTypeRevisionMacro(vtkFollower,vtkActor);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Creates a follower with no camera set
  static vtkFollower *New();

  // Description:
  // Set/Get the camera to follow. If this is not set, then the follower
  // won't know who to follow.
  virtual void SetCamera(vtkCamera*);
  vtkGetObjectMacro(Camera,vtkCamera);

  // Description:
  // This causes the actor to be rendered. It in turn will render the actor's
  // property, texture map and then mapper. If a property hasn't been
  // assigned, then the actor will create one automatically. 
  virtual int RenderOpaqueGeometry(vtkViewport *viewport);
  virtual int RenderTranslucentPolygonalGeometry(vtkViewport *viewport);
  virtual void Render(vtkRenderer *ren);
  
  // Description:
  // Does this prop have some translucent polygonal geometry?
  virtual int HasTranslucentPolygonalGeometry();
  
  // Description:
  // Copy the follower's composite 4x4 matrix into the matrix provided.
  virtual void GetMatrix(vtkMatrix4x4 *m);
  virtual void GetMatrix(double m[16]);
  virtual vtkMatrix4x4* GetMatrix()
    {return this->vtkActor::GetMatrix();}

  // Description:
  // Shallow copy of a follower. Overloads the virtual vtkProp method.
  void ShallowCopy(vtkProp *prop);

protected:
  vtkFollower();
  ~vtkFollower();

  vtkCamera *Camera; 
  vtkActor  *Device;
private:
  // hide the two parameter Render() method from the user and the compiler.
  virtual void Render(vtkRenderer *, vtkMapper *) {};
private:
  vtkFollower(const vtkFollower&);  // Not implemented.
  void operator=(const vtkFollower&);  // Not implemented.
};

#endif



