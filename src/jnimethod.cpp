
#include <jnicpptl/jnimethod.h>
#include <jnicpptl/jniobject.h>
#include <jnicpptl/jniexception.h>


JniMethod_base::JniMethod_base(JniObject* instanceProxy, const char* name, const char* sig)
 : m_methodName(name)
 , m_methodSignature(sig)
 , mCachedMethodID(NULL)
 , mInstanceProxy(instanceProxy)
{
}

JNIEnv* JniMethod_base::getJNIEnv()
{
	if(mEnv == NULL && mInstanceProxy != NULL) {
		return mInstanceProxy->getJNIEnv();
	}
	return mEnv;
}

jclass JniMethod_base::getClass()
{
	return getJNIEnv()->GetObjectClass(getInstance());
}

jobject JniMethod_base::getInstance()
{
	if(mInstance == NULL && mInstanceProxy != NULL) {
		return mInstanceProxy->getInstance();
	}
	return mInstance;
}

jmethodID JniMethod_base::getMethodID()
{
	if(mCachedMethodID == NULL){
		mCachedMethodID = getJNIEnv()->GetMethodID(getClass(), m_methodName, m_methodSignature);
		if(mCachedMethodID == NULL){
			throwErrorNow(getJNIEnv(), "unable to find method %s. (%s)", m_methodName, m_methodSignature);
		}
	}
	return mCachedMethodID;
}