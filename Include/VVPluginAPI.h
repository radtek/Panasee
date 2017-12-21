
#ifndef vtkVVPluginAPI_h
#define vtkVVPluginAPI_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#ifdef  __cplusplus
extern "C" {
#endif
	
	
	/* These types are returned by GetDataType to indicate pixel type. */
#ifndef VTK_VOID  
#define VTK_VOID            0
#define VTK_BIT             1 
#define VTK_CHAR            2
#define VTK_UNSIGNED_CHAR   3
#define VTK_SHORT           4
#define VTK_UNSIGNED_SHORT  5
#define VTK_INT             6
#define VTK_UNSIGNED_INT    7
#define VTK_LONG            8
#define VTK_UNSIGNED_LONG   9
#define VTK_FLOAT          10
#define VTK_DOUBLE         11 
#define VTK_ID_TYPE        12
#endif
	
	/* the following defines enumerate the different GUI components supported by
	* plugins.  */
#define VV_GUI_SCALE     0
#define VVP_GUI_SCALE    "scale"
#define VV_GUI_CHOICE    1
#define VVP_GUI_CHOICE   "choice"
#define VV_GUI_CHECKBOX  2
#define VVP_GUI_CHECKBOX "checkbox"
	
#define VVP_ERROR                         0
#define VVP_NAME                          1
#define VVP_TERSE_DOCUMENTATION           2
#define VVP_FULL_DOCUMENTATION            3
#define VVP_NUMBER_OF_GUI_ITEMS           6
#define VVP_ABORT_PROCESSING             10  
#define VVP_REPORT_TEXT                  11

	
#define VVP_GUI_LABEL   0
#define VVP_GUI_TYPE    1
#define VVP_GUI_DEFAULT 2
#define VVP_GUI_HELP    3
#define VVP_GUI_HINTS   4
#define VVP_GUI_VALUE   5
	
	
	/*=========================================================================
	CM_PLUGIN_EXPORT should be used by the Init function of all plugins
	=========================================================================*/
#ifdef _WIN32
#define VV_PLUGIN_EXPORT  __declspec( dllexport )
#else
#define VV_PLUGIN_EXPORT 
#endif
	
	
	typedef struct 
	{
		/* the input data pointer */
		void *inData; 
		/* the output data pointer */
		void *outData; 
		/* what slice to start processing on (used for pieces) */    
		int StartSlice;         
		/* how many slices to process right now */
		int NumberOfSlicesToProcess;
		
	} vtkVVProcessDataStruct;
	
	typedef struct
	{
				
		/* these are the characteristics of the input data */
		int InputVolumeScalarType;
		int InputVolumeScalarSize;
		int InputVolumeDimensions[3];
		float InputVolumeSpacing[3];
		float InputVolumeOrigin[3];
		double InputVolumeScalarRange[8]; /* actual scalar range */
		double InputVolumeScalarTypeRange[2]; /* possible scalar range */
						
		int NumberOfMarkers;
		float *Markers; /* the xyz positions of the seed points/markers */
		
		/* specify the charateristics of the output data */
		int OutputVolumeScalarType;
		int OutputVolumeDimensions[3];
		float OutputVolumeSpacing[3];
		float OutputVolumeOrigin[3];
		
		/* these are the methods that a plugin can invoke on VolView */
		/* the following function can be called to update progress */
		void  (*UpdateProgress) (void *info, float progress, const char *msg);				
		
		/* memory safe way to set/get properties */
		void        (*SetProperty) (void *info, int property, const char *value);
		const char *(*GetProperty) (void *info, int property);
		void        (*SetGUIProperty) (void *info, int num, int property, 
			const char *value);
		const char *(*GetGUIProperty) (void *info, int num, int property);
		
		/* you must implement the two following functions */
		int (*ProcessData) (void *info, vtkVVProcessDataStruct *pds);
		int (*UpdateGUI)   (void *info);
		
	} vtkVVPluginInfo;
	
	
	typedef void (*VV_INIT_FUNCTION)(vtkVVPluginInfo *);
	
#ifdef  __cplusplus
}
#endif


#endif