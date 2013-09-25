#include <jnicpptl/jnifield.h>
#include <jnicpptl/jniobject.h>
#include <jnicpptl/jniexception.h>

template<bool isStatic>
JniField_base<isStatic>::JniField_base(JniObject* proxy, const char* name, const char* signature)
 : mInstanceProxy(proxy)
 , mFieldName(name)
 , mFieldSignature(signature)
 , mCachedFieldID(NULL)
{

}

template<bool isStatic>
JNIEnv* JniField_base<isStatic>::getJNIEnv()
{
	if(mEnv == NULL && mInstanceProxy != NULL) {
		return mInstanceProxy->getJNIEnv();
	}
	return mEnv;
}

template<bool isStatic>
jclass JniField_base<isStatic>::getClass()
{
	return getJNIEnv()->GetObjectClass(getInstance());
}

template<bool isStatic>
jobject JniField_base<isStatic>::getInstance()
{
	if(mInstance == NULL && mInstanceProxy != NULL) {
		return mInstanceProxy->getInstance();
	}
	return mInstance;
}

template<>
jfieldID JniField_base<true>::getFieldID()
{
	if(mCachedFieldID == NULL) {
		mCachedFieldID = getJNIEnv()->GetStaticFieldID(getClass(), mFieldName, mFieldSignature);
		if(mCachedFieldID == NULL){
			throwErrorNow(getJNIEnv(), "unable to find field %s. (%s)", mFieldName, mFieldSignature);
		}
	}
	return mCachedFieldID;
}

template<>
jfieldID JniField_base<false>::getFieldID()
{
	if(mCachedFieldID == NULL) {
		mCachedFieldID = getJNIEnv()->GetFieldID(getClass(), mFieldName, mFieldSignature);
		if(mCachedFieldID == NULL){
			throwErrorNow(getJNIEnv(), "unable to find field %s. (%s)", mFieldName, mFieldSignature);
		}
	}
	return mCachedFieldID;
}