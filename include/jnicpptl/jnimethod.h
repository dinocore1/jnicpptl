

#ifndef JNIMETHOD_H_
#define JNIMETHOD_H_

#include <jni.h>
#include <cstdio>

class JniObject;

template<class T>
class JniMethod;


class JniMethod_base {

protected:
	const char* m_methodName;
	const char* m_methodSignature;

	JNIEnv* mEnv;
	jobject mInstance;
	jmethodID mCachedMethodID;

	JniObject* mInstanceProxy;
	

public:
	JniMethod_base(const char* name, const char* sig)
	: m_methodName(name)
	, m_methodSignature(sig)
	, mCachedMethodID(NULL)
	{}

	JniMethod_base(JniObject* instanceProxy, const char* name, const char* sig);

	
	void setInstance(JNIEnv* env, jobject instance) {
		mEnv = env;
		mInstance = instance;
	}


	JNIEnv* getJNIEnv();
	jclass getClass();
	jobject getInstance();
	jmethodID getMethodID();

};



#define JniMethod_ACCESSORS(TYPE, NAME_FRAG) \
template<typename... P> class JniMethod<TYPE(P...)> : public JniMethod_base { \
	public: \
	JniMethod(const char* name, const char* sig) : JniMethod_base(name, sig) {}; \
	JniMethod(JniObject* proxy, const char* name, const char* sig) : JniMethod_base(proxy, name, sig) {}; \
	TYPE operator() (P... q) { \
		return (TYPE)getJNIEnv()->Call ## NAME_FRAG ## Method(getInstance(), getMethodID(), q...); \
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
