#if !defined _MY_GEOMETRY_H
#define _MY_GEOMETRY_H

#include "Model/mesh.h"
#include "Model/PointCloud.h"

typedef enum DATA_TYPE {
	DATATYPE_POINTCLOUD,
	DATATYPE_MESH,
	DATATYPE_RECTANGLE,
	DATATYPE_SPHERE,
	DATATYPE_POLYLINE,
	DATATYPE_ELLIPSOID,
	DATATYPE_BOX,
	DATATYPE_SEGMENT,
} MY_GEO_DATATYPE;


#endif