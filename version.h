#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.08";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 1;
	static const long BUILD = 854;
	static const long REVISION = 4869;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 952;
	#define RC_FILEVERSION 0,1,854,4869
	#define RC_FILEVERSION_STRING "0, 1, 854, 4869\0"
	static const char FULLVERSION_STRING[] = "0.1.854.4869";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 824;
	

}
#endif //VERSION_H
