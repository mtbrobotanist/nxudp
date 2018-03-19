#ifndef NXUDP_EXPORT_H_
#define NXUDP_EXPORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32

	#ifdef NXUDP_EXPORTS  
		#define NXUDP_API __declspec(dllexport)   
	#else  
		#define NXUDP_API __declspec(dllimport)   
	#endif  

#else
	#define NXUDP_API 
#endif

#ifdef __cplusplus
}
#endif

#endif // #ifndef NXUDP_EXPORT_H_