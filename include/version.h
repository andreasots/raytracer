#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "31";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.12";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 1;
	static const long BUILD = 2246;
	static const long REVISION = 12375;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2560;
	#define RC_FILEVERSION 0,1,2246,12375
	#define RC_FILEVERSION_STRING "0, 1, 2246, 12375\0"
	static const char FULLVERSION_STRING[] = "0.1.2246.12375";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 2216;
	

}
#endif //VERSION_H
