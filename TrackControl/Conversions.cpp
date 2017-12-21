/*****************************************************************
Name:			Conversions.cpp

Description:    This routine contains all the routines used to
				convert data.


All Northern Digital Inc. ("NDI") Media and/or Sample Code and/or
Sample Code Documentation (collectively referred to as "Sample Code")
is licensed and provided "as is" without warranty of any kind.  The
licensee, by use of the Sample Code, warrants to NDI that the Sample
Code is fit for the use and purpose for which the licensee intends to
use the Sample Code. NDI makes no warranties, express or implied, that
the functions contained in the Sample Code will meet the licensee’s
requirements or that the operation of the programs contained therein
will be error free.  This warranty as expressed herein is exclusive
and NDI expressly disclaims any and all express and/or implied, in fact
or in law, warranties, representations, and conditions of every kind
pertaining in any way to the Sample Code licensed and provided by NDI
hereunder, including without limitation, each warranty and/or condition
of quality, merchantability, description, operation, adequacy,
suitability, fitness for particular purpose, title, interference with
use or enjoyment, and/or non infringement, whether express or implied
by statute, common law, usage of trade, course of dealing, custom, or
otherwise.  No NDI dealer, distributor, agent or employee is authorized
to make any modification or addition to this warranty.

In no event shall NDI nor any of its employees be liable for any direct,
indirect, incidental, special, exemplary, or consequential damages,
sundry damages or any damages whatsoever, including, but not limited to,
procurement of substitute goods or services, loss of use, data or profits,
or business interruption, however caused.  In no event shall NDI’s
liability to the licensee exceed the amount paid by the licensee for the S
ample Code or any NDI products that accompany the Sample Code. The said
imitations and exclusions of liability shall apply whether or not any
such damages are construed as arising from a breach of a representation,
warranty, guarantee, covenant, obligation, condition or fundamental term
or on any theory of liability, whether in contract, strict liability, or
tort (including negligence or otherwise) arising in any way out of the
use of the Sample Code even if advised of the possibility of such damage.
In no event shall NDI be liable for any claims, losses, damages, judgments,
costs, awards, expenses or liabilities of any kind whatsoever arising
directly or indirectly from any injury to person or property, arising
from the Sample Code or any use thereof.


Copyright (C) 2002, 2003, Northern Digital Inc. All rights reserved.


*****************************************************************/


/*****************************************************************
C Library Files Included
*****************************************************************/
#include <stdlib.h>
#include <string.h> 

/*****************************************************************
Project Files Included
*****************************************************************/
#include "APIStructures.h"
#include "conversions.h"

#include "math.h"

/*****************************************************************
Defines
*****************************************************************/
/* None. */

/*****************************************************************
Global Variables
*****************************************************************/
/* None. */


/***************************************************************************
Name:               uASCIIToHex

Input Values:
	char
		szStr[]				:String of ASCII characters that make up a hex
							 integer value to be converted.
							 Note that the string may NOT be null-terminated.
	int
		nLen				:Length of input string.

Output Values:
	None.

Returned Value:
	unsigned int			:Integer equivalent of input array.

Description:
	This routine translates a character ASCII array which is
	hex to its equivalent integer value.

***************************************************************************/
unsigned int uASCIIToHex( char szStr[], int nLen )
{
	char
		chTemp;
	unsigned int
		uVal;
	int
		nCnt;

	uVal = 0;
	for ( nCnt = 0; nCnt < nLen; ++nCnt )
	{
		chTemp = szStr[nCnt];
		/*
		 * Convert hex ASCII digits to values to add to total value.
		 */
		if ( (chTemp >= '0') && (chTemp <= '9') )
		{
			chTemp -= '0';
		} /* if */
		else if ( (chTemp >= 'A') && (chTemp <= 'F') )
		{
			chTemp = 0x0000000a + (chTemp - 'A');
		} /* else if */
		else if ( (chTemp >= 'a') && (chTemp <= 'f') )
		{
			chTemp = 0x0000000a + (chTemp - 'a');
		} /* else if */
		else
		{
			/*
			 * We've hit a non-hex character.
			 */
			return( 0 );
		} /* else */

		/*
		 * Shift result into position of total value.
		 */
		uVal |= (chTemp << (4 * (nLen - 1 - nCnt)));
	} /* for */

	return( uVal );
} /* uASCIIToHex */
/*****************************************************************
Routine:    bExtractValue

Inputs:	pszVal - value to be extracted, uLen - length of value
		fDivisor - how to break up information, pfValue - data

Returns:

Description: This routine breaks up the transformation into
			 there individual components

*****************************************************************/
bool bExtractValue( char *pszVal, unsigned uLen,
					float fDivisor, float *pfValue )
{
    unsigned
        i;
    char
        szBuff[ 10 ];

    *pfValue = BAD_FLOAT;

    /*
     * Make sure that the first character is either a + or -.
     */
    if( *pszVal != '-' && *pszVal != '+' )
	{
      return false;
	} /* if */

    /*
     * Copy the + or - character to the buffer
     */
    szBuff[0] = *pszVal;

    /*
     * Copy the rest of the value.  Make sure that the remainder of
     * the value string contains only digits 0 - 9.
     */
    for( i = 1; i < uLen; i++ )
    {
        if( pszVal[i] < '0' || pszVal[i] > '9' )
		{
             return false;
		} /* if */

        szBuff[i] = pszVal[i];
    } /* for */

    /*
     * Null terminate the string.
     */
    szBuff[i] = '\0';

    *pfValue = float(atof( szBuff ) / fDivisor);
    
    return true;
} /* bExtractValue */
/*****************************************************************
Routine:   nGetHex1

Inputs:		char *sz - the buffer that contains the 2 hex chars
			of information to be decoded
	
Returns:	int u - the decimal value of the decoding
	
Description:
	Converts two (2) hex characters to its decimal equivialent

*****************************************************************/
int nGetHex1(char *sz)
{
    unsigned int u;

    u=sz[0] & 0xff;

    return u;
} /* nGetHex1 */
/*****************************************************************
Routine:   nGetHex2

Inputs:		char *sz - the buffer that contains the 2 hex chars
			of information to be decoded
	
Returns:	int u - the decimal value of the decoding
	
Description:
	Converts two (2) hex characters to its decimal equivialent

*****************************************************************/
int nGetHex2(char *sz)
{
    unsigned int u;

    u=sz[0] & 0xff;
    u|=((sz[1] & 0xFF) << 8);

    return u;
} /* nGetHex2 */
/*****************************************************************
Routine:   nGetHex4

Inputs:		char *sz - the buffer that contains the 4 hex chars
			of information to be decoded
	
Returns:	int u - the decimal value of the decoding
	
Description:
	Converts two (4) hex characters to its decimal equivialent

*****************************************************************/
int nGetHex4(char *sz)
{
    unsigned int u;

    u=sz[0] & 0xff;
    u|=((sz[1] & 0xFF) << 8);
    u|=((sz[2] & 0xFF) << 16);
    u|=((sz[3] & 0xFF) << 24);

    return (int) u;
} /* nGetHex4 */
/*****************************************************************
Routine:   fGetFloat

Inputs:		char *sz - the buffer that contains the 2 hex chars
			of information to be decoded
	
Returns:	int f - the float value of the decoding
	
Description:
	Converts two (2) hex characters to its float equivialent

*****************************************************************/
float fGetFloat(char *sz)
{
    float f;
    unsigned int *pu;

    pu =((unsigned int *)&f);

    (*pu)=sz[0] & 0xff;
    (*pu)|=((sz[1] & 0xFF) << 8);
    (*pu)|=((sz[2] & 0xFF) << 16);
    (*pu)|=((sz[3] & 0xFF) << 24);

    return f;
} /* fGetFloat */

/****** MATH FUNCTIONS FOR EULER AND QUATERNION MATH ******/
/********************************************************************
Name:        CvtQuatToRotationMatrix 

Input Values:
    QuatRotation
        *pdtQuatRot :Ptr to the quaternion rotation. 

Output Values:
    RotationMatrix
        dtRotationMatrix :The 3x3 determined rotation matrix. 

Returned Value:
    None.
Description:
    This routine determines the rotation matrix that corresponds
    to the given quaternion. 

    Let the quaternion be represented by: 

        | Q0 |
    Q = | Qx |
        | Qy |
        | Qz |

    and the rotation matrix by: 

        | M00 M01 M02 |
    M = | M10 M11 M12 |
        | M20 M21 M22 |

    then assuming the quaternion, Q, has been normalized to convert
    Q to M we use the following equations: 

    M00 = (Q0 * Q0) + (Qx * Qx) - (Qy * Qy) - (Qz * Qz)
    M01 = 2 * ((Qx * Qy) - (Q0 * Qz))
    M02 = 2 * ((Qx * Qz) + (Q0 * Qy))
    M10 = 2 * ((Qx * Qy) + (Q0 * Qz))
    M11 = (Q0 * Q0) - (Qx * Qx) + (Qy * Qy) - (Qz * Qz)
    M12 = 2 * ((Qy * Qz) - (Q0 * Qx))
    M20 = 2 * ((Qx * Qz) - (Q0 * Qy))
    M21 = 2 * ((Qy * Qz) + (Q0 * Qx))
    M22 = (Q0 * Q0) - (Qx * Qx) - (Qy * Qy) + (Qz * Qz) 
*********************************************************************/
void CvtQuatToRotationMatrix( QuatRotation *pdtQuatRot,
                RotationMatrix dtRotMatrix )
{ 
    float
        fQ0Q0,
        fQxQx,
        fQyQy,
        fQzQz,
        fQ0Qx,
        fQ0Qy,
        fQ0Qz,
        fQxQy,
        fQxQz,
        fQyQz;

    /*
     * Determine some calculations done more than once.
     */
        fQ0Q0 = pdtQuatRot->q0 * pdtQuatRot->q0;
        fQxQx = pdtQuatRot->qx * pdtQuatRot->qx;
        fQyQy = pdtQuatRot->qy * pdtQuatRot->qy;
        fQzQz = pdtQuatRot->qz * pdtQuatRot->qz;
        fQ0Qx = pdtQuatRot->q0 * pdtQuatRot->qx;
        fQ0Qy = pdtQuatRot->q0 * pdtQuatRot->qy;
        fQ0Qz = pdtQuatRot->q0 * pdtQuatRot->qz;
        fQxQy = pdtQuatRot->qx * pdtQuatRot->qy;
        fQxQz = pdtQuatRot->qx * pdtQuatRot->qz;
        fQyQz = pdtQuatRot->qy * pdtQuatRot->qz; 

    /*
     * Determine the rotation matrix elements.
     */
        dtRotMatrix[0][0] = (float)(fQ0Q0 + fQxQx - fQyQy - fQzQz);
        dtRotMatrix[0][1] = (float)(2.0 * (-fQ0Qz + fQxQy));
        dtRotMatrix[0][2] = (float)(2.0 * (fQ0Qy + fQxQz));
        dtRotMatrix[1][0] = (float)(2.0 * (fQ0Qz + fQxQy));
        dtRotMatrix[1][1] = (float)(fQ0Q0 - fQxQx + fQyQy - fQzQz);
        dtRotMatrix[1][2] = (float)(2.0 * (-fQ0Qx + fQyQz));
        dtRotMatrix[2][0] = (float)(2.0 * (-fQ0Qy + fQxQz));
        dtRotMatrix[2][1] = (float)(2.0 * (fQ0Qx + fQyQz));
        dtRotMatrix[2][2] = (float)(fQ0Q0 - fQxQx - fQyQy + fQzQz); 

} /* CvtQuatToRotationMatrix */ 

/*******************************************************************
Name:         DetermineEuler 

Input Values:
    RotationMatrix
        dtRotationMatrix :The 3x3 rotation matrix to convert. 

Output Values:
    Rotation
        *pdtEulerRot :Rotation is Euler angle format.
            Roll, pitch, yaw Euler angles which define the required rotation. 

Returned Value:
    None. 

Description:
    This routine calculates the Euler angles given the 3x3 rotation matrix. 
*******************************************************************/ 
void DetermineEuler( RotationMatrix dtRotMatrix, Rotation *pdtEulerRot ) 
{ 
    float
        fRoll,
        fCosRoll,
        fSinRoll;

    fRoll =(float) atan2( dtRotMatrix[1][0], dtRotMatrix[0][0] );
    fCosRoll = (float) cos( fRoll );
    fSinRoll = (float) sin( fRoll );

    pdtEulerRot->fRoll = fRoll;
    pdtEulerRot->fPitch = (float) atan2( -dtRotMatrix[2][0],
     (fCosRoll * dtRotMatrix[0][0]) + (fSinRoll *
     dtRotMatrix[1][0]) );
    pdtEulerRot->fYaw = (float) atan2(
     (fSinRoll * dtRotMatrix[0][2]) -
     (fCosRoll * dtRotMatrix[1][2]),
     (-fSinRoll * dtRotMatrix[0][1]) +
     (fCosRoll * dtRotMatrix[1][1]) ); 

} /* DetermineEuler */

/**************************************************************
Name:        CvtQuatToEulerRotation 

Input Values:
    QuatRotation
        *pdtQuatRot :Ptr to the quaternion rotation. 

Output Values:
    Rotation
        *pdtEulerRot :Ptr to the determined rotation Euler angles. 

Returned Value:
    None. 

Description:
    This routine determines the rotation in Euler angles (degrees)that
    corresponds to the given quaternion rotation. 
******************************************************************/
void CvtQuatToEulerRotation( QuatRotation *pdtQuatRot, Rotation *pdtEulerRot )
{ 
    RotationMatrix
        dtRotMatrix; 

    CvtQuatToRotationMatrix( pdtQuatRot, dtRotMatrix ); 

    DetermineEuler( dtRotMatrix, pdtEulerRot ); 

    pdtEulerRot->fYaw *= (float) RAD_TO_DEGREES;
    pdtEulerRot->fPitch *= (float) RAD_TO_DEGREES;
    pdtEulerRot->fRoll *= (float) RAD_TO_DEGREES;

} /* CvtQuatToEulerRotation */ 

/*****************************************************************
Name:               QuatRotatePoint

Input Values:
    QuatRotation
        *RotationQuaternionPtr: pointer to the quaternion
                                  defining the rotation

    QuatVector3dStruct
        *OriginalPositionPtr  : pointer to struct containing the
                                  original position of the given point
        *RotatedPositionPtr   : pointer to struct containing the
                                  rotated position of the given point

Output Values:
    The vector pointed to by RotatedPositionPtr is over written
    with the calculated position of the point after rotation.

Returned Value:
    None

Description:
    This routine calculates the 3-D position of a given point after
    being rotated by a rotation defined by the given quaternion

*****************************************************************/
void QuatRotatePoint( QuatRotation *RotationQuaternionPtr, 
					  Position3d   *OriginalPositionPtr,
					  Position3d   *RotatedPositionPtr )
{
    Position3d
        UCrossV;      /* The cross product of the vector part of the rotation
                           quaternion with the original position vector */

    if( OriginalPositionPtr->x < MAX_NEGATIVE ||
        OriginalPositionPtr->y < MAX_NEGATIVE ||
        OriginalPositionPtr->z < MAX_NEGATIVE )
    {
        RotatedPositionPtr->x =
        RotatedPositionPtr->y =
        RotatedPositionPtr->z = BAD_FLOAT;

        return;
    } /* if */

    UCrossV.x = RotationQuaternionPtr->qy * OriginalPositionPtr->z
                - RotationQuaternionPtr->qz * OriginalPositionPtr->y;
    UCrossV.y = RotationQuaternionPtr->qz * OriginalPositionPtr->x
                - RotationQuaternionPtr->qx * OriginalPositionPtr->z;
    UCrossV.z = RotationQuaternionPtr->qx * OriginalPositionPtr->y
                - RotationQuaternionPtr->qy * OriginalPositionPtr->x;

    RotatedPositionPtr->x = float(OriginalPositionPtr->x
                                + 2.0 * ( RotationQuaternionPtr->q0*UCrossV.x
                                        + RotationQuaternionPtr->qy*UCrossV.z
                                        - RotationQuaternionPtr->qz*UCrossV.y ));
    RotatedPositionPtr->y = float(OriginalPositionPtr->y
                                + 2.0 * ( RotationQuaternionPtr->q0*UCrossV.y
                                        + RotationQuaternionPtr->qz*UCrossV.x
                                        - RotationQuaternionPtr->qx*UCrossV.z ));
    RotatedPositionPtr->z = float(OriginalPositionPtr->z
                                + 2.0 * ( RotationQuaternionPtr->q0*UCrossV.z
                                        + RotationQuaternionPtr->qx*UCrossV.y
                                        - RotationQuaternionPtr->qy*UCrossV.x ));
} /* QuatRotatePoint */

/*****************************************************************
Name:            QuatInverseXfrm

Input Values:
    QuatTransformation
       *pdtXfrm             : input transformation

Output Values:
    QuatTransformation
       *pdtNewXfrm          : new inversed transformation

Returned Value:
    None

Description:
    This routine determines the inverse transform to the given
    transformation

*****************************************************************/
void QuatInverseXfrm(QuatTransformation *pdtXfrm, QuatTransformation *pdtNewXfrm)
{
    pdtNewXfrm->rotation.q0 =  pdtXfrm->rotation.q0;
    pdtNewXfrm->rotation.qx = -pdtXfrm->rotation.qx;
    pdtNewXfrm->rotation.qy = -pdtXfrm->rotation.qy;
    pdtNewXfrm->rotation.qz = -pdtXfrm->rotation.qz;

    QuatRotatePoint( &pdtNewXfrm->rotation, &pdtXfrm->translation,
                              &pdtNewXfrm->translation );

    pdtNewXfrm->translation.x = -pdtNewXfrm->translation.x;
    pdtNewXfrm->translation.y = -pdtNewXfrm->translation.y;
    pdtNewXfrm->translation.z = -pdtNewXfrm->translation.z;
} /* QuatInverseXfrm */



/*****************************************************************
Name:            QuatCombineXfrms

Input Values:
    QuatTransformation
       *pdtXfrm12           : Transformation from RF 1 to RF 2
       *pdtXfrm23           : Transformation from RF 2 to RF 3

Output Values:
    QuatTransformation
       *pdtXfrm13           : Output Transformation from RF 1 to RF 3

Returned Value:
    None

Description:
    This routine combines the input transformations from RF 1 to RF 2
    and RF 2 to RF 3 to generate a new transformation from RF 1 to RF 3
*****************************************************************/
void QuatCombineXfrms( QuatTransformation *pdtXfrm12,
					  QuatTransformation *pdtXfrm23,
					  QuatTransformation *pdtXfrm13 )
{
    QuatRotation
        *pdtQ12 = &pdtXfrm12->rotation,
        *pdtQ23 = &pdtXfrm23->rotation,
        *pdtQ13 = &pdtXfrm13->rotation;
    float
        fA,    /* some useful temporary variables */
        fB,
        fC,
        fD,
        fE,
        fF,
        fG,
        fH;

    fA = ( pdtQ23->q0 + pdtQ23->qx ) * ( pdtQ12->q0 + pdtQ12->qx );
    fB = ( pdtQ23->qz - pdtQ23->qy ) * ( pdtQ12->qy - pdtQ12->qz );
    fC = ( pdtQ23->qx - pdtQ23->q0 ) * ( pdtQ12->qy + pdtQ12->qz );
    fD = ( pdtQ23->qy + pdtQ23->qz ) * ( pdtQ12->qx - pdtQ12->q0 );
    fE = ( pdtQ23->qx + pdtQ23->qz ) * ( pdtQ12->qx + pdtQ12->qy );
    fF = ( pdtQ23->qx - pdtQ23->qz ) * ( pdtQ12->qx - pdtQ12->qy );
    fG = ( pdtQ23->q0 + pdtQ23->qy ) * ( pdtQ12->q0 - pdtQ12->qz );
    fH = ( pdtQ23->q0 - pdtQ23->qy ) * ( pdtQ12->q0 + pdtQ12->qz );

    pdtQ13->q0 =  float (fB + ( -fE - fF + fG + fH ) / 2.0);
    pdtQ13->qx =  float (fA - (  fE + fF + fG + fH ) / 2.0);
    pdtQ13->qy = float (-fC + (  fE - fF + fG - fH ) / 2.0);
    pdtQ13->qz = float (-fD + (  fE - fF - fG + fH ) / 2.0);

    QuatRotatePoint( &pdtXfrm23->rotation, &pdtXfrm12->translation,
                             &pdtXfrm13->translation );
    pdtXfrm13->translation.x += pdtXfrm23->translation.x;
    pdtXfrm13->translation.y += pdtXfrm23->translation.y;
    pdtXfrm13->translation.z += pdtXfrm23->translation.z;
} /* QuatCombineXfrms */
/**************************END OF FILE***************************/
