
#ifndef JNIEXCEPTION_H_
#define JNIEXCEPTION_H_

#include <jni.h>


class JniObject;

void throwNow(JNIEnv* env, const jclass &exceptionclass, const char* fmt, va_list arg);
void throwNow(JNIEnv* env, const jclass &exceptionclass, const char* fmt, ...);
void throwNow(JNIEnv* env, JniObject &type, const char* fmt, va_list arg);
void throwErrorNow(JNIEnv* env, const char* fmt, ...);


#endif /* JNIEXCEPTION_H_ */
