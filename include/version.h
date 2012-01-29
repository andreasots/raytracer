#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "29";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.01";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 1;
	static const long BUILD = 2570;
	static const long REVISION = 14156;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2973;
	#define RC_FILEVERSION 0,1,2570,14156
	#define RC_FILEVERSION_STRING "0, 1, 2570, 14156\0"
	static const char FULLVERSION_STRING[] = "0.1.2570.14156";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 2540;
	

}
#endif //VERSION_H
