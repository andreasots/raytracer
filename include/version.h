#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.10";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 1;
	static const long BUILD = 1793;
	static const long REVISION = 9903;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2022;
	#define RC_FILEVERSION 0,1,1793,9903
	#define RC_FILEVERSION_STRING "0, 1, 1793, 9903\0"
	static const char FULLVERSION_STRING[] = "0.1.1793.9903";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 1763;
	

}
#endif //VERSION_H
