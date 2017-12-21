// MyMath.h: wrapper class for basic math calculation 
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_MYMATH_H__4AC3D190_94EC_11D2_B21D_00C04FBEDB8F__INCLUDED_)
#define AFX_MYMATH_H__4AC3D190_94EC_11D2_B21D_00C04FBEDB8F__INCLUDED_

#define TEST_DEMO
#define MAX_TRINUM    500000
#define EPSLON        1.0e-6


/* *****************************************************************************
 * MatrixInverse is used to calculate the inverse of a square matrix
 * Input: 
 *      A -- pointer to a  square matrix 
 * Output: 
 *      A  -- pointer to a square matrix after inverse 
 * Return Void
 *
 * Note:
 * Calculation is based on SDV algorithm
 *  A = [U].[diag(w)][Vt] by SDV algorithm
 * inv(A)  = [V].[diag(1/w)].[Ut]
 *      
 * The previous function InverseMatrix() is actually a transpose of a orthogonal matrix.
 *
 *  Author: 
 *       Q. Tang  11.11.00
 * **********************************************************************************/

/* *****************************************************************************
 * DLT2Pos() is used to calculate the Camera position from 11 DLT parameters
 * Input: 
 *      dlt_par[11] -- 11 DLT parameters L1, L2, ...L11  as double
 * Output: 
 *      pos[3]  -- camera position (x,y,z)
 * Return Void
 *
 * Note:
 * Calculation is based on the following equation
 *      
 * (x,y,z)' = inv((M)*(-L4, -L8, -1)';
 *  Where M = [L1 L2 L3; L5 L6 L7; L9, L10, L11)];
 *
 *  Author: 
 *       Q. Tang  11.11.00
 * **********************************************************************************/

/* *****************************************************************************
 * DLT2Ori() is used to calculate the Camera rotation matrix from 11 DLT parameters
 * Input: 
 *      dlt_par[11] -- 11 DLT parameters L1, L2, ...L11  as double
 * Output: 
 *      ori[3][3]  -- camera rotation matrix (3x3 )
 * Return void
 * 
 * Note:
 * Calculation is based on the following equation
 *      
 *  b1 = (L1 L2 L3)'; 
 *  b2 = (L5 L6 L7)';
 *  b3 = (L9 L10 L11)';
 *  x0 = b1*b3/(b3*b3);
 *  y0 = b2*b3/(b3*b30l
 *  cx = sqrt((b1*b1)/(b3*b3) - x0*x0]);
 *  cy = sqrt((b2*b2)/B3*b3 - y0*y0);
 *  b1' = (b1-x0*b3)/cx;
 *  b2' = (b2-y0*b3)/cy;
 *  The rotation matrix M is obtained by
 *
 *  M = (-b1'/norm(b1')  -b2'/norm(b2') b3/norm(b3))';  
 *
 * Author: 
 *       Q. Tang  11.11.00
 * **********************************************************************************/

/* *****************************************************************************
 * DLT2CamPos() is used to calculate the Camera position structurefrom 11 DLT parameters
 * Input: 
 *      dlt_par[11] -- 11 DLT parameters L1, L2, ...L11  as double
 * Output: 
 *      cam_pos  -- camera position matrix
 * Return void
 * 
 * Note:
 * Calculation is based on the following equation
 *  (x,y,z)' = inv((M)*(-L4, -L8, -1)';
 *  Where M = [L1 L2 L3; L5 L6 L7; L9, L10, L11)];
 *      
 *  b1 = (L1 L2 L3)'; 
 *  b2 = (L5 L6 L7)';
 *  b3 = (L9 L10 L11)';
 *  x0 = b1*b3/(b3*b3);
 *  y0 = b2*b3/(b3*b30l
 *  cx = sqrt((b1*b1)/(b3*b3) - x0*x0]);
 *  cy = sqrt((b2*b2)/B3*b3 - y0*y0);
 *  b1' = (b1-x0*b3)/cx;
 *  b2' = (b2-y0*b3)/cy;
 *  The rotation matrix M is obtained by
 *
 *  M = (-b1'/norm(b1')  -b2'/norm(b2') b3/norm(b3))';  
 *
 * The relation between camera and reference coordinate (turntable) systems is
 * Xt = M*Xc + X0;  or Xc = M'*(Xt-X0); 
 * where Xt is the turntable coodinates and Xc is the camera coodinates. 
 * Xo is the camera position in the turntabe coodinate (cam_pos->x, cam_pos->y, cam_pos->z)
 * M is the rotation matrix from camera to table coodinate  (cam_pos->m)
 * 
 * Author: 
 *       Q. Tang  11.19.00
 * **********************************************************************************/

/* *****************************************************************************
 * DLT2CamPos2() is used to calculate the Camera position structure from 11 DLT2 parameters
 * DLT2 are obtained assuming that principle axis positions are given
 * Input: 
 *      dlt_par[11] -- 11 DLT parameters L1, L2, ...L11  as double
 *      center[2] -- principle axis position on CCD (for example, (320, 240) 
 *                   This number is assumed to be known in finding DLT2 
 * Output: 
 *      cam_pos  -- camera position matrix2 (without principel axis position x0 and yo)
 * Return void
 * 
 * Note:
 * The equation is similar to DLT2CamPos except for setting x0 = 0 and y0 = 0
 * Calculation is based on the following equation
 *  (x,y,z)' = inv((M)*(-L4, -L8, -1)';
 *  Where M = [L1 L2 L3; L5 L6 L7; L9, L10, L11)];
 *      
 *  b1 = (L1 L2 L3)'; 
 *  b2 = (L5 L6 L7)';
 *  b3 = (L9 L10 L11)';
 *  cx = sqrt((b1*b1)/(b3*b3));
 *  cy = sqrt((b2*b2)/B3*b3);
 *  b1' = (b1-x0*b3)/cx;
 *  b2' = (b2-y0*b3)/cy;
 *  The rotation matrix M is obtained by
 *
 *  M = (-b1'/norm(b1')  -b2'/norm(b2') b3/norm(b3))';  
 *
 * The relation between camera and reference coordinate (turntable) systems is
 * Xt = M*Xc + X0;  or Xc = M'*(Xt-X0); 
 * where Xt is the turntable coodinates and Xc is the camera coodinates. 
 * Xo is the camera position in the turntabe coodinate (cam_pos->x, cam_pos->y, cam_pos->z)
 * M is the rotation matrix from camera to table coodinate  (cam_pos->m)
 * 
 * Author: 
 *       Q. Tang  11.19.00
 * **********************************************************************************/

/*****************************************************************************************
 * CamPos2DLT() is used to calculate the 11 DLT parameters from Camera position structure
 * Input: 
 *      cam_pos  -- camera position structure
 * Output: 
 *      dlt_par[11] -- 11 DLT parameters L1, L2, ...L11  as double
 * Return void
 * 
 * Note:
 * Calculation is based on the following equation
 *  L = -(m13*Xcr + m23*Ycr + m33*Zcr);

 *	L1 = (x0*m13-cx*m11)/L;
 *	L2 = (x0*m23-cx*m21)/L;
 *	L3 = (x0*m33-cx*m31)/L;
 *	L4 = -L1*Xcr-L2*Ycr -L3*Zcr;
 *	L5 = (y0*m13-cy*m12)/L;
 *	L6 = (y0*m23-cy*m22)/L;
 *	L7 = (y0*m33-cy*m32)/L;
 *	L8 = -L5*Xcr-L6*Ycr -L7*Zcr;
 *	L9 = m13/L;
 *	L10 = m23/L;
 *	L11 = m33/L;

 *  DLT = [L1 L2 L3 L4 L5 L6 L7 L8 L9 L10 L11]';

 * where (x0,y0) are the principle axis position of the lens on the detector in pixel 
 * M is the rotation matrix from table to camera coodinate 
 * (Xcr, Ycr, Zcr) are the camera position on the turntable coodinate
 * 
 * Author: 
 *       Q. Tang  12.4.00
 * **********************************************************************************/

/* *****************************************************************************
 * CalibrateDLT () is used to calibrate/calculate the DLT parameters of a camera
 * Input: 
 *      Two sets of calibration data
 *      int_num -- number of input points
 *      pt_3d[3] -- a 3d point with (x,y,z) coordinate
 *      pt_2d[2]  -- corresponding position in the image plane
 * Output: 
 *      dlt_par[11] -- 11 DLT parameters L1, L2, ...L11  as double
 * Return void
 * 
 * Note:calibration is based on the LS fitting of the following equation
 *     
 *  x = (L1*X+L2*Y+L3*Z+L4)/(L9*X+L10*Y+L11*Z+1);
 *  y = (L5*X+L6*Y+L7*Z+L8)/(L9*X+L10*Y+L11*Z+1);
 * 
 * Given a set of (X,Y,Z) and (x,y) find (L1,...L11);
 * A least 6 points are required to determine the DLT parameters. 
 * The measured points (control points) can not be co-planar to void sigularity of the matrix
 *
 * This equation can be re-written as the form
 * AX = B
 * where 
 * A is 2*num_pointx11 matrix 
 * A = [ X1	 Y1	Z1	1	0	0	0	0	-x1X1	-x1Y1	-x1Z1
 *        0	 0  0	0	X1	Y1	Z1	1	-y1X1	-y1Y1	-y1Z1
 *		  .	.	.	.	.	.	.	.	.	.	.	.	.	.
 *		  .	.	.	.	.	.	.	.	.	.	.	.	.	.
 *		 Xm	 Ym	Zm	1	0	0	0	0	-xmXm	-xmYm	-xmZm
 *        0	 0  0	0	Xm	Ym	Zm	1	-ymXm	-ymYm	-ymZm];
 
 * B is 2*num_point x 1 vector
 * B = [x1 y1 .... xm ym]';
 * And X is DLT parameters
 * X = [L1 L2 L3 L4 L5 L6 L7 L8 L9 L10 L11]';
 *
 * Note: (X,Y,Z) has physical unit (mm or inch). (x,y) has unit of pixel. 
 * Scale due to camera focal length and CCD pixel size is refelected by DLT parameters.
 *
 * Author: 
 *       Q. Tang  11.19.00
 * **********************************************************************************/

/* *****************************************************************************
 * CalibrateDLT2 () is used to calibrate/calculate the DLT parameters of a camera
 * Input: 
 *      Two sets of calibration data
 *      int_num -- number of input points
 *      pt_3d[3] -- a 3d point with (x,y,z) coordinate
 *      pt_2d[2]  -- corresponding position in the image plane
 *      center[2] -- principe axis position in CCD arrya in pixel. For example, 320, 240
 * Output: 
 *      dlt_par[11] -- 11 DLT parameters L1, L2, ...L11  as double
 * Return void
 * 
 * Note:calibration is based on the LS fitting of the following equation
 *     
 *  x = (L1*X+L2*Y+L3*Z+L4)/(L9*X+L10*Y+L11*Z+1);
 *  y = (L5*X+L6*Y+L7*Z+L8)/(L9*X+L10*Y+L11*Z+1);
 * 
 * where x = pt_2d[0] - center[0];
 *       y = pt_2d[1] - center[1];
 *
 * Given a set of (X,Y,Z) and (x,y) find (L1,...L11);
 * A least 6 points are required to determine the DLT parameters. 
 * The measured points (control points) can not be co-planar to void sigularity of the matrix
 *
 * This equation can be re-written as the form
 * AX = B
 * where 
 * A is 2*num_pointx11 matrix 
 * A = [ X1	 Y1	Z1	1	0	0	0	0	-x1X1	-x1Y1	-x1Z1
 *        0	 0  0	0	X1	Y1	Z1	1	-y1X1	-y1Y1	-y1Z1
 *		  .	.	.	.	.	.	.	.	.	.	.	.	.	.
 *		  .	.	.	.	.	.	.	.	.	.	.	.	.	.
 *		 Xm	 Ym	Zm	1	0	0	0	0	-xmXm	-xmYm	-xmZm
 *        0	 0  0	0	Xm	Ym	Zm	1	-ymXm	-ymYm	-ymZm];
 
 * B is 2*num_point x 1 vector
 * B = [x1 y1 .... xm ym]';
 * And X is DLT parameters
 * X = [L1 L2 L3 L4 L5 L6 L7 L8 L9 L10 L11]';
 *
 * Note: (X,Y,Z) has physical unit (mm or inch). (x,y) has unit of pixel. 
 * Scale due to camera focal length and CCD pixel size is refelected by DLT parameters.
 *
 * Author: 
 *       Q. Tang  11.19.00
 * **********************************************************************************/

/* *****************************************************************************
 * FLT2CamPos() is used to calculate the Camera position structurefrom 8 FLT parameters
 * Input: 
 *      flt_par[11] -- 8 FLT parameters a1, a2, ...a11  as double
 * Output: 
 *      cam_pos  -- camera position matrix
 * Return void
 * 
 * Note:
 * Calculation is based on the following equation
 *
 *
 * The relation between camera and reference coordinate (turntable) systems is
 * Xt = M*Xc + X0;  or Xc = M'*(Xt-X0); 
 * where Xt is the turntable coodinates and Xc is the camera coodinates. 
 * Xo is the camera position in the turntabe coodinate (cam_pos->x, cam_pos->y, cam_pos->z)
 * M is the rotation matrix from camera to table coodinate  (cam_pos->m)
 * 
 * Author: 
 *       Q. Tang  12.14.00
 * **********************************************************************************/

/* *****************************************************************************
 * CalibrateFLT () is used to calibrate/calculate the FLT parameters of a camera
 * Input: 
 *      Two sets of calibration data
 *      int_num -- number of input points
 *      pt_3d[2] -- a 3d point with (x,y,0) coordinate (they are in one plane)
 *      pt_2d[2]  -- corresponding position in the image plane
 * Output: 
 *      flt_par[8] -- 8 DLT parameters a1, a2, ..., a8  as double
 * Return void
 * 
 * Note:calibration is based on the LS fitting of the following equation
 *     
 *  x-x0 = (a1*X+a2*Y+a3)/(a7*X+a8*Y+1);
 *  y-y0 = (a4*X+a5*Y+a6)/(a7*X+a8*Y+1);
 * 
 * Given a set of (X,Y,0) and (x,y) find (a1, a2, ...a8);
 * A least 6 points are required to determine the FLT parameters. 
 * The measured points (control points) are co-planar
 *
 * This equation can be re-written as the form
 * AX = B
 * where 
 * A is 2*num_pointx11 matrix 
 * A = [ X1	 Y1	1	0	0	0	-x1X1	-x1Y1
 *        0	 0 	0	X1	Y1	1	-y1X1	-y1Y1
 *		  .	.		.		.	.	.	.	.	
 *		  .	.		.	.	.	.	.	.	.	
 *		 Xm	 Ym	1	0	0	0	-xmXm	-xmYm	
 *        0	 0 	0	Xm	Ym	1	-ymXm	-ymYm	];
 
 * B is 2*num_point x 1 vector
 * B = [x1 y1 .... xm ym]'
 * where x1 = x1 - x0;
 *       y1 = y1 - y0;
 * (x0,y0) is the position of principle axis in CCD (i.e. 320, 240)
 * And X is FLT parameters
 * X = [a1 a2 a3 a4 a5 a6 a7 a8]';
 *
 * Note: (X,Y,0) has physical unit (mm or inch). (x,y) has unit of pixel. 
 * Scale due to camera focal length and CCD pixel size is refelected by FLT parameters.
 *
 * Author: 
 *       Q. Tang  12.12.00
 * **********************************************************************************/

/* *****************************************************************************
 * DLT3DProj() is used to project a 3d point (x,y,z) into the image plane based on
 * 11 DLT parameters
 * Input: 
 *      dlt_par[11] -- 11 DLT parameters L1, L2, ...L11  as double
 *      pt_3d[3] -- a 3d point with (x,y,z) coordinate
 * Output: 
 *      pt_2d[2]  -- position in the image plane
 * Return void
 * 
 * Note:
 * Calculation is based on the following equation
 * 2d image coordinate (x,y) is obtained from 3d coodinate (X,Y,Z)     
 *  x = (L1*X+L2*Y+L3*Z+L4)/(L9*X+L10*Y+L11*Z+1);
 *  y = (L5*X+L6*Y+L7*Z+L8)/(L9*X+L10*Y+L11*Z+1);
 * 
 * Note: (X,Y,Z) has physical unit (mm or inch). (x,y) has unit of pixel. 
 * Scale due to camera focal length and CCD pixel size is refelected by DLT parameters.
 *
 * Author: 
 *       Q. Tang  11.11.00
 * **********************************************************************************/

/* *****************************************************************************
 * Atan360() is used to calculate arctan() value with the range 0 - 360
 * It is the modification for standard function atan() that results in the value from -PI/2 to PI/2
 *
 * Input: 
 *      numerator (num)
 *      denominator (den) to form triangle
 * Output: 
 *      arc tangent value with value from 0 to 360 degree
 * Return 
 *       Output
 *
 * Author: 
 *       Q. Tang  12.04.00
 * **********************************************************************************/

/* *****************************************************************************
 * CamPTR2Matrix(doube ptr[3], double ori[3][3]) trnsfer camera pan, tilt, roll angles 
 * into rotation matrix
 *
 * Input: 
 *      Camera orienation parameter ptr array [pan, tilt, roll]
 * Output: 
 *      Camera oritation matrix ori
 * Return 
 *       void
 *
 * Author: 
 *       Q. Tang  12.04.00
 * **********************************************************************************/

/* *****************************************************************************
 * CamMatrix2PTR(double ori[3][3],doube ptr[3]) transfer rotation matrix to
 * camera pan, tilt, roll angles 
 *
 * Input: 
 *		Camera oritation matrix ori
 *
 * Output: 
 *      Camera orienation parameter ptr array [pan, tilt, roll]
 * Return 
 *       void
 * Note: The calculation is based on the folling relations
 * pan = atan360(Cam_M(3,1),Cam_M(3,3))*180/pi;
 * temp = (Cam_M(1,2)^2 + Cam_M(2,2)^2)^.5;
 * tilt = atan(-Cam_M(3,2)/temp)*180/pi;
 * roll = atan360(Cam_M(1,2),Cam_M(2,2))*180/pi;
 *
 * Author: 
 *       Q. Tang  12.04.00
 * **********************************************************************************/

/* *****************************************************************************
 * RotateDLT(double dlt1[11], double angle, double dlt2[11]) 
 * Give a set of DLT parameters (dlt1) 
 * and rotation angle along Y axis (Pan)
 * find the resulting DLT parameters (dlt2)
 *
 * Input: 
 *		DLT parameters (dlt1[11]) and rotating angle (angle)
 *
 * Output: 
 *      New DLT parameter after rotation (dlt2[11])
 * Return 
 *       void
 * Note: The calculation is based on the rotation matrix along Y axis
 * My = [cos(angle) 0 -sin(angle)
 *  	0          1       0
 *     sin(angle) 0       cos(angle)]; 
 *
 * Author: 
 *       Q. Tang  12.04.00
 * **********************************************************************************/

/*******************************************************************************************
 * GetCyliner() calculates the cylinder central axis and its radius based on input points 
 * by using nonlinear least sqaure method
 * Inputs: 
 *        p[][3]; containing a measured point array of [x,y,z];
 *        num_point; number of pints of the input; 
 * Outputs:  
 *         axis normal (nx, ny nz) and the center (x0,y0,z0) of input points that is on axis.
 *         and radius R 
 *         standard devaiation (stdDev) and maxmium deviation (maxDev) 
 *                    
 * return:
 *         void
 * 
 * Notes:
 * Since data fitting is obtained by nonlinear least method that requires initial values
 * To avoid not giving initial value certain procedure has to be followed during the measuremenet
 * Measurement approach
 * 1. measure points around one end of the cylinder. The measurements should be
 *    taken approximately on a section of the cylinder
 * 2. Measure points around the other end of the cylinder. The measuremnts should be
 *    taken approximately on a section of the cylinder
 * The initial [x,y,z] is the central points of the first measurement
 * The initial radius R is the average distance between the initial center and measured points
 * The intial axis normal is determined by the connection of two central points
 * Assume that the number of measurement points for two sections are equal that is num_point/2;
 * 
 * Author: 
 *         Q. Tang 10.29.00 
 */





/////////////////////// CLASS CMyMath  /////////////////
typedef struct _Cam_Pos{
	// exterior parameter
	double x;          // camera  position x 
	double y;          // camera  position y
	double z;          // camera  position z
	double m[3][3];    // camera  rotation matrix
    // interior parameter
	double x0;         // optical principla position in CCD in unit of pixel
	double y0;         // optical principal position in CCD in unit of pixle
	double cx;         // scale factor = f/pixel_size in horizontal
	double cy;         // scale factor = f/pixel_size in vertical 
} CAM_POS;

typedef struct _PT_3D {
	double x;
	double y;
	double z;
} PT_3D;


// only used to support 3d circle fitting function
typedef struct _PT_RPY {
	double x;
	double y;
	double z;
	double roll;
	double pitch;
	double yaw;
} PT_RPY;

#define CAL_DLT 1
#define CAL_FLT 2


class CPGMath 
{
public:
	void AxisRotation(double xyz[], double rot_ang, double nx, double ny, double nz, double xyz_out[]);
	void AxisRotation(double xyz[3], double rot_ang, double axis[6], double xyz_out[3]);
    void LineFitting2D(int     numPnt, 
                       double *coefficients, 
                       double *x, 
                       double *y, 
                       double *stdev);
	void MinEigenvector(double **A, int nrow, double *eigenVector);
	void PlaneFitting(int num_point, double  *a[3], double *nx, double *ny, double *nz, double *d, double *stdDev, double *maxDev);
	void SampleCovarianceAndMean(int numSample, int dim, double *sampleData[], double *mean, double **cov);
	void Det_CoefficientByDltAndXy(double dlt[], double xy[], double *Coeff_of_A, double *Coeff_of_B, double *Coeff_of_C);
	void DetZ_CoefficientByDltAndXy(double dlt[], double xy[], double *constant, double *Coeff_of_A, double *Coeff_of_B);
	void DetY_CoefficientByDltAndXy(double dlt[], double xy[], double *constant, double *Coeff_of_A, double *Coeff_of_C);
	void DetX_CoefficientByDltAndXy(double dlt[11], double xy[2], double *constant, double *Coeff_of_B, double *Coeff_of_C);
	void GetLaserPos(int num_point, double *xyl, double dlt[11], double z0, 
		double k[3], double laser_pos[4], double &std, double &maxError);
	void GetLaserPos2(int num_point, double *xyl, double dlt[11], double x0, 
		double k[3], double laser_pos[4], double &std, double &maxError);
	void LineFitting3D(int num_point, double *a[], double *kx, double *ky, double *kz, double *std, double *maxErr);
	double DistanceFromPointToLine(double x, double y, double z, double *lineDirection, double x0, double y0, double z0);

	void MaxEigenvector(double **A, int nrow, double *eigenVector);
	void LineFitting3D(int num_point, double *a[], 
                        double *kx, double *ky, double *kz, 
                        double *x0, double *y0, double *z0, 
                        double *std, double *maxErr);
	double d;
	void EllipseFitting3D(int numPoint, 
	                      double *ellipse[3], 
						  int fittedPoint, 
						  double *newEllipse[3]);
	void EllipseFitting3D(int     numPoint, 
							   double *ellipse[3], 
							   int     fittedPoint,
							   double *newEllipse[3],
							   double *cx,
							   double *cy,
							   double *cz,
							   double *xAxi, 
							   double *yAxi, 
							   double *Theta);
	void Transfer2Dto3D(int numPoint, double *Point2D[2], double* Point3D[3]);
	double nz;
	double ny;
	double nx;
	double yaw;
	double pitch;
	double roll;
	void EllipseFitting_2D(int numPoint, double *ellipse[2], int fittedPoint, double *newEllipse[2]);
	void EllipseFitting_2D(int     numPoint, 
	                       double *ellipse[2], 
						   int     fittedPoint,
						   double *newEllipse[2],
						   double  *xc, 
						   double  *yc,  
						   double  *xAxi, 
						   double  *yAxi, 
						   double  *Theta);
 
	void CopyMatrix(double m1[], int startrow1,int startcol1, double m2[],
					int startrow2, int startcol2, int rows, int cols);

	void MultiplyMatrixFloat(double m1[], int row1, int col1, double m2[], 
		int col2, double m3[]);

	void MultiplyMatrix(double m1[], int row1, int col1, double m2[], 
						int col2, double m3[]);

	void LinearLeastSquare(int numRow, int numCol, double **A, double *b, double *x, double *stdDev = 0, double *maxError = 0);
 
	void NonLinearLeastSquare(double** x, double y[], double sig[], int ndata, 
			double a[], int ma, int lista[], int mfit, double** covar, 
			double** alpha, double* chisq, double* alamda, int index);
	void NonLinearLeastSquare(double** x, double y[], double sig[], int ndata, 
			double a[], int ma, int lista[], int mfit, int index);

	void NonLinearLeastSquare_calib(double** x, double y[], double sig[], int ndata, 
			double a[], int ma, int lista[], int mfit, int index, double *pXYZw);

	void NonLinearLeastSquare_calib_I(double** x, double y[], double sig[], int ndata, 
			double a[], int ma, int lista[], int mfit, int index, double *pXYZw);

//	void NonLinearLeastSquare_modelfit(double** x, double y[], double sig[], int ndata, 
//		double a[], double *fiterror, int ma, int lista[], int mfit, int num_line, double **line_pars);

	void NonLinearLeastSquare_modelfit(double** x, double y[], double sig[], int ndata, 
		double a[], double fiterror[2], int ma, int lista[], int mfit, int num_line, double **line_pars);
		
	
	void DLT2Pos(double dlt_par[11], double pos[3]);
	void DLT2Ori(double dlt_par[11], double ori[3][3]);
	void MatrixInverse(int numCol, double **A);
	void DLT3DProj(double dlt_par[11], double pt_3d[3], double pt_2d[2]);
	void DLT2CamPos(double dlt_par[11],CAM_POS *cam_pos);
	void DLT2CamPos2(double dlt_par[11],double center[2], CAM_POS *cam_pos);
	void DLT2CamPos2(double dlt_par[11],double center[2], double scale[2], CAM_POS *cam_pos);
	void DLT2CamPos3(double dlt_par[11],CAM_POS *cam_pos, double center[2]);
	void CamPos2DLT(CAM_POS *cam_pos, double dlt_par[11]);
	void CalibrateDLT(int num_point, double *pt_3d[3], double *pt_2d[2], double plt_par[11], double *stdDev = 0, double *maxErr = 0);
	void CalibrateDLT2(int num_point, double *pt_3d[3], double *pt_2d[2], double center[2], double plt_par[11],double *stdDev = 0, double *maxErr = 0);
	void CalculateTargetTrans(int num_point, double *pt_3d[3], double *pt_2d[2], double dlt_par[11], double center[2], double trans[3], double *stdDev, double *maxError);
	void CalibrateFLT(int num_point, double *pt_3d[2], double *pt_2d[2], double center[2], double flt_par[8]);
	void CalibrateFLT2(int num_point, double *pt_3d[2], double *pt_2d[2], double center[2], double cxy[2], double flt_par[8]);
	double Atan360(double num, double den);
	void CamPTR2Matrix(double ptr[3], double ori[3][3]);
	void CamYRP2Matrix(double ptr[3], double ori[3][3]);
	void CamMatrix2PTR(double ori[3][3], double ptr[3]);
	void CamMatrixYRP(double ori[3][3], double ptr[3]);
	void RotateDLT(double dlt1[11], double angle, double dlt2[11]);
	void RotateDLT_Z(double dlt1[11], double angle, double dlt2[11]);
	void TranslateDLT(double dlt1[11], double x, double y, double z, double dlt2[11]);
	void TransformDLT(double dlt1[11], double T[4][4], double dlt2[11], double pdImgCenter[2]);
	void FLT2CamPos(double flt_par[8],double center[2], CAM_POS *cam_pos);
	void FLT2CamPos2(double flt_par[8],double center[2], double cxy[2], CAM_POS *cam_pos);

	void MatrixMultiply33(double m1[3][3],double m2[3][3], double m3[3][3]);
	void MatrixMultiply44(double m1[4][4],double m2[4][4], double m3[4][4]);
	void MatrixMultiplyVector44(double m1[4][4],double v1[4], double v2[4]);
	void MatrixMultiplyVector33(double m1[3][3],double v1[3], double v2[3]);

    // The following functions are used to support 3d circle fitting 
    void CircleFitting(int num_point, double *cir[3], double *xc, double *yc, double *zc, double *R, double *Nx, double *Ny, double *Nz);
    void CircleFitting(int num_point, double *cir[3], double *xc, double *yc, double *zc, double *R, double *Nx, double *Ny, double *Nz, double *stdDev, double *maxDev);
	void GetPlaneNormal(int num_point, double  *a[3], double *nx, double *ny, double *nz, double *d);
	void GetPlaneNormal(int num_point, double  *a[3], double *nx, double *ny, double *nz, double *d, double *stdDev, double *maxDev);
	void GetSphere(int num_point, double  *p[3], double init_par[4], double *x0, double *y0, double *z0, double *R, double *stdDev, double *maxDev);
	void GetSphere_fixRadius(int num_point, double  *p[3], double init_par[4], double *x0, double *y0, double *z0, double *R, double *stdDev, double *maxDev);
	void Get2DCircleFit_NL(int num_point, double  *p[2], double init_par[3], double *x0, double *y0, double *R, double *stdDev, double *maxDev, bool bFixedRadius);
	void GetCylinder(int num_point, double  *p[3], double *nx, double *ny, double *nz, double *x0, double *y0, double *z0, double *R, double *stdDev, double *maxDev);
    void GetXShiftParams(int num_point, double  *p[5], double init_par[3], double *k1, double *k2, double *k3, double *stdDev, double *maxDev);
	void GetXShiftParams(int num_point, double  *p[5], double init_par[6], double *k1, double *k2, double *k3, double *x0, double *y0, double *z0, double *stdDev, double *maxDev);
    void GetXShiftParams2(int num_point, double  *p[5], double init_par[6], double *k1, double *k2, double *k3, double *x0, double *y0, double *z0, double *stdDev, double *maxDev);
	void GetXShiftAndRotateParams(int num_point, double  *p[6], double init_par[8], double *k1, double *k2, double *k3, double *x0, double *y0, double *z0, double *rot_x, double *rot_z, double *stdDev, double *maxDev);
	void GetXShiftAndRotateParams2(int num_point, double  *p[6], double init_par[8], double *k1, double *k2, double *k3, double *x0, double *y0, double *z0, double *rot_x, double *rot_z, double *stdDev, double *maxDev);
    void GetXShiftAndRotateParams3(int num_point, double  *p[6], double init_par[8], double *k1, double *k2, double *k3, double *x0, double *y0, double *z0, double *rot_x, double *rot_z, double *stdDev, double *maxDev);
	void GetXShiftAndRotateParams4(int num_point, double  *p[6], double init_par[9], double *k1, double *k2, double *k3, double *x0, double *y0, double *z0, double *rot_x, double *rot_z, double *del_theta, double *stdDev, double *maxDev);
	void GetXShiftAndRotateParams5(int num_point, double  *p[6], double init_par[8], double *k1, double *k2, double *k3, double *x0, double *y0, double *z0, double *rot_x, double *rot_z, double *stdDev, double *maxDev);
	void CircleFitting_2D (int num_point, double *cir[2], double *xc, double *yc, double *R);
	void CircleFitting_2D (int num_point, double *cir[2], double *xc, double *yc, double *R, double *stdDev, double *maxDev);
	void CircleFitting_3D_NL(int num_point, double *cir[3], double *xc, double *yc, double *zc, double *R, double *Nx, double *Ny, double *Nz, double *stdDev, double *maxDev, bool bFixedRadius);
		
	PT_RPY PTIJK_to_PTRPY( double P1[], double d[], double n[]);

	int CalOrgInCamSys(double m[3][3], double n[3], double ori[3], int mode);
	int CalOrgInCamSys2(double m[3][3], double cam[3], double ori[3]);

    int CalculateRotAxis(double m[3][3], double n[3]);
    
	int  CalculateCoordinatefrom2Vectors(double vector_y[3], double orig[3], double vector_x[3], double t[4][4]);
	int  CalCamPosfromTabCoor(double t[4][4], double cxy[2], double center[2], CAM_POS *cam_pos, double dlt[11] );
	int CalculateRotCenter(double p1[3], double p2[3], double n[3], double c[3], double beta);
	int CalculateRotCenter(CAM_POS *pos1, CAM_POS *pos2, double beta, double c[3], double n[3], int mode);
	int CalculateRotCoor(CAM_POS *pos1, CAM_POS *pos2, double beta, double t[4][4], int mode);
	double CalculateRotAxisBy2Mat(double m1[3][3], double m2[3][3], double n[3]);
  
	// Basic transform
	void GetTabPosFromCamPos(CAM_POS *cam_pos, PT_3D *tab_pos);
	CAM_POS *CalibrateCamPosByFLT(double *pt_2d[2], double center[2], double cxy[2]);
	void CalibrateFLT_NPProc(double *pt_2d[2], double *pt_3d[3], double center[2], double cxy[2], double flt_par[8], CAM_POS *cam_pos, double cam_ptr[3], int dis_mode = 0);
    void TwoDLT2Trid(double dlt1[11], double dlt2[11], double pxy1[2], double pxy2[2], double pt_3d[3]);
	void MDLT2Trid(int iNumImg, double *pDLT[11], double *pt_2d[2], double pt_3d[3]);
    void RotateYAxis(double xyz_in[3], double angle, double xyz_out[3]);
    void LaserPlaneInCamCoor(CAM_POS *cam_pos, double pl_tab[4], double pl_cam[4]);
	void LaserPlaneInTabCoor(CAM_POS *cam_pos, double pl_cam[4], double pl_tab[4]);
	void PointInCamCoor(CAM_POS *cam_pos, double pxyz_tab[3], double pxyz_cam[3]);
	void PointInTabCoor(CAM_POS *cam_pos, double pxyz_cam[3], double pxyz_tab[3]);

    // basic geometry (projection)
	void PointProjection(double x1, double y1, double z1, double nx, double ny, double nz, double d, double *x2, double *y2, double *z2);
    void PointLineProjection(double x0, double y0, double z0, double x1, double y1, double z1, double nx, double ny, double nz,double *xp, double *yp, double *zp);
	void PointLineDistance(double x0, double y0, double z0, double x1, double y1, double z1, double nx, double ny, double nz,double *d);
	void PointLineDistance_2D(double x0, double y0, double line_mode, double a, double b, double *d);
	void LinePlaneIntersection(double xl, double yl, double zl, double nlx, double nly, double nlz, double npx, double npy, double npz, double d, double *xi, double *yi, double *zi);
	//  line1_par = [x1, y1, z1, nx1, ny1, nz1]
	//  line2_par = [x2, y2, z2, nx2, ny2, nz2]
	void Line2LineIntersect(double line1_par[6], double line2_par[6], double intersect[3], double *err);
		
	bool TransposeMatrix(double  m1[], int row, int col, double m2[]);
	bool CalculateTransform(int iNum, double T1[], double T2[], double M[16]);
	bool CalculateTransform2(int iNum, double T1[], double T2[], double M[16]);
	bool CalculateTransform3(double T1[], double T2[], double M[16]);

    void CloseLoopCalib(int num_point, int num_par, double  *cd[9], double init_par[],  bool FitSelect[], double calib_pars[]);
    void CloseLoopCalib_I(int num_point, int num_par, double  *cd[9], double init_par[],  bool FitSelect[], double calib_pars[],int PlanNum);
	void GetRotationAxisByPlanes(int num_point, double  *p[8], double init_par[6], double cal_par[6], double *stdDev, double *maxDev);
	void GetDLT_LaserByPlanes(int num_point, double  *p[9], double init_par[15], double cal_par[15], bool bDLT_Laser, double *stdDev, double *maxDev);
    void laser_center_correction(double laser_width, double dlt[11], double &delta_u0);


	// Misc.
	bool Normalize(double &nx,double &ny, double &nz);
	double ran0(int *idum);
	double gaussdev(int *idum);
	void GetModelTranform(int num_point, double  *p[3], int num_line, double **line_pars, double init_par[6], double eval_par[6], double *stdDev, double *maxDev);
	void ConvertAngle2Matrix(double alphe, double beta, double gamma, double tx, double ty, double tz, double t[4][4]);		
	

	void CalibrateDLT_NPProc(int num_point, double *pt_2d[2], double *pt_3d[3], double dlt_par[11], CAM_POS *cam_pos, double cam_ptr[3], double lens_distortion[], int dis_mode = 0);
	void CalibrateDLT_NPProc2(int num_point, double *pt_2d[], double *pt_3d[], double dlt_par[], CAM_POS *cam_pos, double cam_ptr[], double lens_distortion[], int dis_mode = 0);
	void CalibrateNLS_randomize(int num_point, double *pt_2d[], double *pt_3d[], CAM_POS *cam_pos, double cam_ptr[], double lens_distortion[]);
	void Calibrate_NLS(int num_point, double *pt_2d[], double *pt_3d[], CAM_POS *cam_pos, double cam_ptr[], double lens_distortion[], double *chi0);
	bool CorrectLensDistortion(double xy_in[2], double xy_out[2], double center[2], double lens_dis[5]);
	void CalibrateDLT_NLS_randomize(int num_point, double *pt_2d[], double *pt_3d[], int mfit,  double init_dlt_par[11], double dlt[11], double lens_distortion[], double lens_center[2]);
	void CalibrateDLT_NLS_randomize_fixCenter(int num_point, double *pt_2d[], double *pt_3d[], int mfit,  double init_dlt_par[11], double dlt[11], double lens_distortion[], double lens_center[2]);
		
	
/*******************************************************************************************
 * create circle points based on its parameters
 * Inputs: 
 *         center position (xc, yc,zc), 
 *         radius R
 *         orientation (nx, ny, nz) in space
 *         number of points (N)
 *         out put format (iyk or rpy); rpy_flag
 * Outputs:
 *         cir[][6]   containing a array of [x,y,z,nx,ny,nz] or [x,y,z r,p,y];
 * return:
 *         void
 * Author: 
 *         Q. Tang 5.17.00 
 */
	void CreateCirclePoints(double xc, double yc, double zc, double R, double nx, double ny, double nz, int N, int rpy_flag, double cir[][6]);
    void GetCylinder(int num_point, double  *p[3], double int_ori[3], double *nx, double *ny, double *nz, double *x0, double *y0, double *z0, double *R, double *stdDev, double *maxDev);


	// Rotate the input points Point3D[i][3]
	// into the plane that is in parellel with xy plane (Z value are the same)
	// input: points in 3D space (they may lay on a plane): Point3D[]
	// output: points after transform: *Point3D[] in which all Z are the same
	//         its rotation angles are *rpy[3]
	void Rotate3DinZ(int numPoint, double *Point3D[], double rpy[3]);

	// Rotate 3D points Point3D[i][3]
	// with angles rpy[3]
	// input/out: Point3D[i][3]
	void Rotate3D(int numPoint, double *Point3D[], double rpy[3]);
	
	//Given a circle parameters (xc,yc,zc,R,nx,ny,nz)
	// create Arc points arc[N][6]
	// arc portion is defined by three sample point samplePt[9][3];
	void CreateArcPoints(double xc, double yc, double zc, double R, double nx, double ny, double nz, int N, double samplePt[9], int rpy_flag, double arc[][6]);
		

	void RegisterationPoints2Points(double **PtsFrom, double **PtsTo, long numPts, double **matrix, double *ms);
	

	// The following functions are used to calculate the distance between
	// a point to a triangle

	bool IsLeagleTriangle(double* pdTriangle, double dMinDistance);

	/*Function: Calculate square distance of a point to a triangle by
	input three point coordinate.
	return : double as square distance.*/
	double gCalcSqrDistPtToTriangle(double* pdPt, double* pdTriangle,
									double *pdS, double* pdT);

	/*Function: Calculate square distance of a point to a triangle by
	input base point coordinate, edge0 and edge1 coordinate.
	return : double as square distance.*/
	double gCalcSqrDistPtToTriangle1(double* pdPt, double* pdTriangle,
									 double *pdS, double* pdT);

	/*Function: Calculate distance of a point to a triangle  by
	input three point coordinate.*/
	double gCalcDistPtToTriangle(double* pdPt, double* pdTriangle);

	/*Function: Calculate distance of a point to a triangle and output
			   the matched point's coordinate in the triangle by input 
			   three triangle's point coordinate.*/
	double gCalcDistPtToTriangleWithMp(double* pdPt, double* pdTriangle,
									   double *pdMp);

	/*Function: Calculate max-edge length of all input triangle.*/
	double gMaxLengthOfAllTriangle(int iTriNum, double* pdTriangle,double *pdAveLen);

	/*Function: Calculate a point is located at which side of a triangle.*/
	double gPtWithTriangle(double *pdPt,double* pdTriangle);

	/*Function: Calculate the errors of all input point to the surface 
	which is composed of all input triangles.*/
	bool gCalcError(int iPtNum, int iTriNum, double *pdPtTab,double* pdTriTab,
					double dMaxErr,double dHitValue);

private:
	void Transfer3Dto2D(int numPoint, double* Point3D[3], double* Point2D[2]);
}; 

#endif // !defined(AFX_MYMATH_H__4AC3D190_94EC_11D2_B21D_00C04FBEDB8F__INCLUDED_)
