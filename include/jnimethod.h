

#ifndef JNIMETHOD_H_
#define JNIMETHOD_H_

#include <jni.h>
#include <cstdio>

#include <jniexception.h>

template<class T>
class JniMethod;


class JniMethod_base {

protected:
	const char* m_methodName;
	const char* m_methodSignature;

	JNIEnv* m_env;
	jobject m_instance;
	jmethodID m_cachedMethodID;
	

public:
	JniMethod_base(const char* name, const char* sig)
	: m_methodName(name)
	, m_methodSignature(sig)
	, m_cachedMethodID(NULL)
	{}

	
	void setInstance(JNIEnv* env, jobject instance) {
		m_env = env;
		m_instance = instance;
	}


	jmethodID getMethodID() {
		if(m_cachedMethodID == NULL){
			m_cachedMethodID = m_env->GetMethodID(getObjectClass(), m_methodName, m_methodSignature);
			if(m_cachedMethodID == NULL){
				JniException::throwErrorNow(m_env, "unable to find method %s. (%s)", m_methodName, m_methodSignature);
			}
		}
		return m_cachedMethodID;
	}

	jclass getObjectClass() const {
        return m_env->GetObjectClass(m_instance);
    }	
	
};



#define JniMethod_ACCESSORS(TYPE, NAME_FRAG) \
template<typename... P> class JniMethod<TYPE(P...)> : public JniMethod_base { \
	public: \
	JniMethod(const char* name, const char* sig) : JniMethod_base(name, sig) {}; \
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
