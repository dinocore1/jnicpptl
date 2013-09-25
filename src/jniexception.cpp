
#include <jnicpptl/jniexception.h>
#include <jnicpptl/jniobject.h>

#define MESSAGE_MAX 1024

void throwNow(JNIEnv* env, const jclass &exceptionclass, const char* fmt, va_list arg)
{
	char buffer[MESSAGE_MAX];
	vsnprintf (buffer, MESSAGE_MAX, fmt, arg);
	env->ThrowNew(exceptionclass, buffer);
}

void throwNow(JNIEnv* env, const jclass &exceptionclass, const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	throwNow(env, exceptionclass, fmt, args);
	va_end(args);
}

void throwNow(JNIEnv* env, JniObject &type, const char* fmt, va_list arg)
{
	throwNow(env, type.getClass(), fmt, arg);	
}


void throwErrorNow(JNIEnv* env, const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	JniObject obj("java/lang/Error", env, NULL);
	throwNow(env, obj, fmt, args);
	va_end(args);
}