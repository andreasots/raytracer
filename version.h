#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "14";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.07";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 0;
	static const long BUILD = 263;
	static const long REVISION = 1516;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 289;
	#define RC_FILEVERSION 0,0,263,1516
	#define RC_FILEVERSION_STRING "0, 0, 263, 1516\0"
	static const char FULLVERSION_STRING[] = "0.0.263.1516";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 263;
	

}
#endif //VERSION_H
