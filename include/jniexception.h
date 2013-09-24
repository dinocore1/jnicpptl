
#ifndef JNIEXCEPTION_H_
#define JNIEXCEPTION_H_

#include <jni.h>


#include <jniobject.h>

#define MESSAGE_MAX 1024

class JniException {

public:

	static void throwNow(JNIEnv* env, const jclass &exceptionclass, const char* fmt, va_list arg) {
		char buffer[MESSAGE_MAX];
		vsnprintf (buffer, MESSAGE_MAX, fmt, arg);
		env->ThrowNew(exceptionclass, buffer);
	}

	static void throwNow(JNIEnv* env, const jclass &exceptionclass, const char* fmt, ...) {
		va_list args;
		va_start (args, fmt);
		throwNow(env, exceptionclass, fmt, args);
		va_end(args);
	}

	static void throwNow(JNIEnv* env, JniObject &type, const char* fmt, va_list arg) {
		throwNow(env, type.getClass(), fmt, arg);	
	}


	static void throwErrorNow(JNIEnv* env, const char* fmt, ...) {
		va_list args;
		va_start (args, fmt);
		JniObject obj(env, "java/lang/Error");
		throwNow(env, obj, fmt, args);
		va_end(args);
	}

};

#endif /* JNIEXCEPTION_H_ */