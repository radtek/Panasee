#ifndef _ERRORCODES_H
#define _ERRORCODES_H

enum ERR_CODE
{
	ERR_OK,	// The operation is successfully done
	ERR_NO_POINTCLOUD,	// No point cloud
	ERR_NO_MESH,		// No mesh
	ERR_NO_PLANE,		// No plane
	ERR_NO_RECT,        // No Rect //WuSh adds 2006-09-07
	ERR_NO_SEGMENT,
	ERR_PLANE_PARA_INVALID,
	ERR_NO_INTERSECTION_BETWEEN_LINES, // No intersection between two lines
	ERR_LINES_ARE_PARALLEL, // The two lines are parallel
	ERR_LINES_ARE_IDENTICAL, // The two lines are identical
	ERR_LINES_ARE_NOT_ON_A_SAME_PLANE, // The two lines are not on a same plane
	ERR_LINE_AND_PLANE_ARE_PARALLEL, // The line and the plane are parallel
	ERR_LINE_IS_ON_THE_PLANE, // The line is just on the plane
	ERR_NO_INTERSECTION_BETWEEN_POINTCLOUD_AND_PLANE, // No intersection between the point cloud and the plane
	ERR_FAIL_TO_DO_INTERSECTION_OF_POINTCLOUD_AND_PLANE,	// Fail to do the intersection operation of the point cloud and the plane
	ERR_NO_INTERSECTION_BETWEEN_MESH_AND_PLANE, // No intersection between the mesh and the plane
	ERR_NO_INTERSECTION_BETWEEN_MESH_AND_LINE, // No intersection between the mesh and the line
	ERR_NO_INTERSECTION_BETWEEN_LINE_AND_TRIANGLE, // No intersection between the line and the triangle
	ERR_MESH_IS_NOT_CLOSED_ALONG_THE_PLANE,					// There is one or more holes on the mesh along the plane
	ERR_FAIL_TO_DO_INTERSECTION_OF_MESH_AND_PLANE,	// Fail to do the intersection operation of the mesh and the plane
	ERR_FAIL_TO_OPEN_FILE,	// Fail to open the file
	ERR_FAIL_TO_SAVE_FILE,	// Fail to save the file
	ERR_NO_POINTS,			// No points
	ERR_NOT_ENOUGH_POINTS,	// Not enough points
	ERR_NO_VALUES,			// No values
	ERR_SEGMENT_THE_TWO_ENDPOINTS_ARE_IDENTICAL,	// The two endpoints are identical
	ERR_FAIL_TO_GENERATE_MESH_FROM_POINTCLOUD,		// Fail to generate mesh from point cloud
	ERR_FAIL_TO_CONSTRUCT_MESH_DATA_FROM_VERTEX_LIST_AND_FACE_LIST, // Fail to construct mesh data from vertex list and face list
	ERR_INVALID_PARAMETER_FOR_RECTANGLE, // Invalid parameter for a rectangle
	ERR_GL_INVALID_OPERATION,	// Invalid parameter for OpenGL operation
	ERR_INVALID_SPACING_VALUE,	// Invalid spacing value: spacing <= 0
	ERR_FAIL_TO_DO_GRID_SAMPLE,	// Fail to do grid sample
	ERR_EXCEED_ITERATION_TIME_LIMIT, // Exceed the iteration time limit
	ERR_INVALID_INDEX,	// Invalid index
	ERR_THREE_FIEXED_POINTS_COLINEAR, // Three fixed points are co-linear
	ERR_THREE_FLOATING_POINTS_COLINEAR, // Three floating points are co-linear
	ERR_PROJECTION_IS_A_POINT, // The projection is just a point
	ERR_RECTANGLE_PARA_INVALID, // Invalid parameters for construction a rectangle
	ERR_POINT_IS_OUTSIDE_RECTANGLE, // The point is outside the rectangle
	ERR_INTERSECTION_AREA_IS_OUTSIDE_RECTANGLE, // The intersection area is outside the rectangle
	ERR_CYLINDER_ABOVE_RECTANGLE, // The cylinder is above the rectangle
	ERR_CYLINDER_BELOW_RECTANGLE, // The cylinder is below the rectangle
	// MRI
	ERR_FAIL_TO_LOAD_IMAGE, // Fail to load image
	ERR_INVALID_NUMBER_OF_POINTS, // Invalid number of points
	ERR_POINT_NUMBERS_NOT_EQUAL, // The sizes of two point sets are not equal
	ERR_FAIL_TO_INITIALIZE_COM, // Fail to initialize COM
	ERR_CIRCLE_NUMBER_NOT_EQUAL,	// The sizes of the three list are not equal
	ERR_INVALID_MRISERIES, //Invalid MRISeries
	ERR_FAIL_TO_DETECT_CIRCLE, //Fail to detect circle
};

#endif