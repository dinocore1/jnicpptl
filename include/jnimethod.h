

#ifndef JNIMETHOD_H_
#define JNIMETHOD_H_

#include <jni.h>
#include <cstdio>

template<class T, bool isStatic>
class JniMethod;

template<typename R, typename... P, bool isStatic>
class JniMethod<R(P...), isStatic> {

	JNIEnv* m_env;
	jobject m_instance;
	const char* m_methodName;
	const char* m_methodSignature;

public:
	JniMethod(){}

	R operator()(P... q);

	jmethodID getMethodID() {
		jmethodID retval = isStatic ? 
			m_env->GetStaticMethodID(getObjectClass(), m_methodName, m_methodSignature) :
			m_env->GetMethodID(getObjectClass(), m_methodName, m_methodSignature);

		if (retval == 0) {
				m_env->ExceptionClear();
				char w[1024];
				sprintf(w,"unable to find method %s. (%s)", m_methodName, m_methodSignature );
				m_env->ThrowNew( m_env->FindClass("java/lang/Error"),w);
        }
        return retval;
	}

private:
	jclass getObjectClass() const {
        return m_env->GetObjectClass(m_instance);
    }
	
};

/*
#define JniMethod_ACCESSORS(TYPE, NAME_FRAG) \
template<class P> TYPE JniMethod<TYPE P, false>::operator()(P... q) { \
	return m_env->Call ## NAME_FRAG ## Method(m_instance, getMethodID(), q); \
}

JniMethod_ACCESSORS(jstring, Object)
JniMethod_ACCESSORS(jobject, Object)
JniMethod_ACCESSORS(jboolean, Boolean)
JniMethod_ACCESSORS(jbyte, Byte)
JniMethod_ACCESSORS(jchar, Char)
JniMethod_ACCESSORS(jshort, Short)
JniMethod_ACCESSORS(jint, Int)
JniMethod_ACCESSORS(jlong, Long)
JniMethod_ACCESSORS(jfloat, Float)
JniMethod_ACCESSORS(jdouble, Double)
*/


#endif /* JNIMETHOD_H_ */
