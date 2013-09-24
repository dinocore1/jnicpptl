

#ifndef JNIMETHOD_H_
#define JNIMETHOD_H_

#include <jni.h>
#include <cstdio>

template<class T>
class JniMethod;


class JniMethod_base {

protected:
	JNIEnv* m_env;
	jobject m_instance;
	jmethodID m_cachedMethodID;
	const char* m_methodName;
	const char* m_methodSignature;

public:
	JniMethod_base(){}

	JniMethod_base(JNIEnv* env, jobject inst, const char* name, const char* sig)
	: m_env(env),
	m_instance(inst),
	m_methodName(name),
	m_methodSignature(sig),
	m_cachedMethodID(NULL) {}


	jmethodID getMethodID() {
		jmethodID retval = m_cachedMethodID;
		if(retval == NULL) {
			jmethodID retval = m_env->GetMethodID(getObjectClass(), m_methodName, m_methodSignature);
			if (retval == NULL) {
					m_env->ExceptionClear();
					char w[1024];
					sprintf(w,"unable to find method %s. (%s)", m_methodName, m_methodSignature );
					m_env->ThrowNew( m_env->FindClass("java/lang/Error"),w);
	        }
	        m_cachedMethodID = retval;
		}

        return retval;
	}

	jclass getObjectClass() const {
        return m_env->GetObjectClass(m_instance);
    }	
	
};



#define JniMethod_ACCESSORS(TYPE, NAME_FRAG) \
template<typename... P> class JniMethod<TYPE(P...)> : public JniMethod_base { \
	public: \
	JniMethod() : JniMethod_base() {}; \
	JniMethod(JNIEnv* env, jobject inst, const char* name, const char* sig) : JniMethod_base(env, inst, name, sig) {} \
	TYPE operator() (P... q) { \
		return m_env->Call ## NAME_FRAG ## Method(m_instance, getMethodID(), q...); \
	} \
};


JniMethod_ACCESSORS(void, Void)
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




#endif /* JNIMETHOD_H_ */
