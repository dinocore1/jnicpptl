#include <jnicpptl/jnifield.h>
#include <jnicpptl/jniobject.h>
#include <jnicpptl/jniexception.h>



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

#define JniField_ACCESSORS(TYPE, NAME_FRAGMENT) \
template<> \
TYPE JniField<TYPE, false>::get() { \
	return (TYPE)getJNIEnv()->Get ## NAME_FRAGMENT ## Field(getInstance(), getFieldID()); \
} \
\
template<> \
TYPE JniField<TYPE, true>::get() { \
	return (TYPE)getJNIEnv()->GetStatic ## NAME_FRAGMENT ## Field(getClass(), getFieldID()); \
} \
\
template<> \
void JniField<TYPE, false>::set(TYPE t) { \
	getJNIEnv()->Set ## NAME_FRAGMENT ## Field(getInstance(), getFieldID(), t); \
} \
\
template<> \
void JniField<TYPE, true>::set(TYPE t) { \
	getJNIEnv()->SetStatic ## NAME_FRAGMENT ## Field(getClass(), getFieldID(), t); \
}

JniField_ACCESSORS(jstring, Object)
JniField_ACCESSORS(jobject, Object)
JniField_ACCESSORS(jboolean, Boolean)
JniField_ACCESSORS(jbyte, Byte)
JniField_ACCESSORS(jchar, Char)
JniField_ACCESSORS(jshort, Short)
JniField_ACCESSORS(jint, Int)
JniField_ACCESSORS(jlong, Long)
JniField_ACCESSORS(jfloat, Float)
JniField_ACCESSORS(jdouble, Double)

#undef JniField_ACCESSORS