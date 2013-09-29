#include <jnicpptl/jnifield.h>
#include <jnicpptl/jniproxy.h>
#include <jnicpptl/jniobject.h>
#include <jnicpptl/jniexception.h>

#define JniField_ACCESSORS(TYPE, NAME_FRAGMENT) \
template<> \
TYPE JniGetStaticField(JNIEnv* env, jclass clazz, jfieldID field) { \
	return (TYPE)env->GetStatic ## NAME_FRAGMENT ## Field(clazz, field); \
} \
\
template<> \
void JniSetStaticField(JNIEnv* env, jclass clazz, jfieldID field, TYPE value) { \
	env->SetStatic ## NAME_FRAGMENT ## Field(clazz, field, value); \
} \
\
template<> \
TYPE JniGetField(JNIEnv* env, jobject obj, jfieldID field) { \
	return (TYPE)env->Get ## NAME_FRAGMENT ## Field(obj, field); \
} \
\
template<> \
void JniSetField(JNIEnv* env, jobject obj, jfieldID field, TYPE value) { \
	env->Set ## NAME_FRAGMENT ## Field(obj, field, value); \
}

JniField_ACCESSORS(jobject, Object)
JniField_ACCESSORS(jstring, Object)
JniField_ACCESSORS(jclass, Object)
JniField_ACCESSORS(jboolean, Boolean)
JniField_ACCESSORS(jbyte, Byte)
JniField_ACCESSORS(jchar, Char)
JniField_ACCESSORS(jshort, Short)
JniField_ACCESSORS(jint, Int)
JniField_ACCESSORS(jlong, Long)
JniField_ACCESSORS(jfloat, Float)
JniField_ACCESSORS(jdouble, Double)

#undef JniField_ACCESSORS

/****** Static JniField_base ******/

template<typename T>
jfieldID JniField_base<T, true>::getFieldID(JNIEnv* env, jclass clazz) {
	if(mFieldID == NULL) {
		mFieldID = env->GetStaticFieldID(clazz, mFieldName.c_str(), mFieldSignature.c_str());
		if(mFieldID == NULL){
			throwErrorNow(env, "unable to find static field %s. (%s)", mFieldName.c_str(), mFieldSignature.c_str());
		}
	}
	return mFieldID;
}

template<typename T>
T JniField_base<T, true>::get(JNIEnv* env, jclass clazz)
{
	if(mFieldID == NULL) {
		mFieldID = getFieldID(env, clazz);
	}
	return JniGetStaticField<T>(env, clazz, mFieldID);
}

template<typename T>
void JniField_base<T, true>::set(JNIEnv* env, jclass clazz, T value)
{
	if(mFieldID == NULL) {
		mFieldID = getFieldID(env, clazz);
	}
	return JniSetStaticField<T>(env, clazz, mFieldID, value);
}


/****** Member JniField_base *******/

template<typename T>
jfieldID JniField_base<T, false>::getFieldID(JNIEnv* env, jclass clazz)
{
	if(mFieldID == NULL) {
		mFieldID = env->GetFieldID(clazz, mFieldName.c_str(), mFieldSignature.c_str());
		if(mFieldID == NULL){
			throwErrorNow(env, "unable to find member field %s. (%s)", mFieldName.c_str(), mFieldSignature.c_str());
		}
	}
	return mFieldID;
}

template<typename T>
T JniField_base<T, false>::get(JNIEnv* env, jobject obj)
{
	if(mFieldID == NULL) {
		mFieldID = getFieldID(env, env->GetObjectClass(obj));
	}
	return JniGetField<T>(env, obj, mFieldID);
}

template<typename T>
void JniField_base<T, false>::set(JNIEnv* env, jobject obj, T value)
{
	if(mFieldID == NULL){
		mFieldID = getFieldID(env, env->GetObjectClass(obj));
	}
	JniSetField<T>(env, obj, mFieldID, value);
}

/****** JniField *******/

template<typename T>
jfieldID JniField<T, true>::getFieldID()
{
	return getFieldID(mProxy->getJNIEnv(), mProxy->getClass());
}

template<typename T>
T JniField<T, true>::get()
{
	return get(mProxy->getJNIEnv(), mProxy->getClass());
}

template<typename T>
void JniField<T, true>::set(T value)
{
	set(mProxy->getJNIEnv(), mProxy->getClass(), value);
}

template<typename T>
jfieldID JniField<T, false>::getFieldID()
{
	return getFieldID(mProxy->getJNIEnv(), mProxy->getClass());
}

template<typename T>
T JniField<T, false>::get()
{
	return get(mProxy->getJNIEnv(), mProxy->getInstance());
}

template<typename T>
void JniField<T, false>::set(T value)
{
	set(mProxy->getJNIEnv(), mProxy->getInstance(), value);
}

/*


#define JniField_ACCESSORS(TYPE, NAME_FRAGMENT) \
template<typename TYPE, true> \
TYPE JniField_base::get(JNIEnv* env, jclass clazz) { \
	jfieldID fieldId = getFieldID(env, clazz); \
	return (TYPE) env->GetStatic ## NAME_FRAGMENT ## Field(clazz, fieldId); \
} \
template<typename TYPE, true> \
void JniField_base::set(JNIEnv* env, jclass clazz, TYPE value) { \
	jfieldID fieldId = getFieldID(env, clazz); \
	env->SetStatic ## NAME_FRAGMENT ## Field(clazz, fieldID, value); \
} \
template<typename TYPE, false> \
TYPE JniField_base::get(JNIEnv* env, jobject obj) { \
	jfieldID fieldId = getFieldID(env, clazz); \
	return (TYPE) env->Get ## NAME_FRAGMENT ## Field(clazz, fieldId); \
} \
template<typename TYPE, false> \
void JniField_base::set(JNIEnv* env, jobject obj, TYPE value) { \
	jfieldID fieldId = getFieldID(env, clazz); \
	env->Set ## NAME_FRAGMENT ## Field(clazz, fieldID, value); \
} \



template<typename T>
jfieldID JniField<T, true>::getFieldID()
{
	if(mCachedFieldID == NULL) {
		mCachedFieldID = getJNIEnv()->GetStaticFieldID(getClass(), mFieldName, mFieldSignature);
		if(mCachedFieldID == NULL){
			throwErrorNow(getJNIEnv(), "unable to find field %s. (%s)", mFieldName, mFieldSignature);
		}
	}
	return mCachedFieldID;
}

template<typename T>
jfieldID JniField<T, false>::getFieldID()
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

JniField_ACCESSORS(jobject, Object)
JniField_ACCESSORS(jstring, Object)
JniField_ACCESSORS(jclass, Object)
JniField_ACCESSORS(jboolean, Boolean)
JniField_ACCESSORS(jbyte, Byte)
JniField_ACCESSORS(jchar, Char)
JniField_ACCESSORS(jshort, Short)
JniField_ACCESSORS(jint, Int)
JniField_ACCESSORS(jlong, Long)
JniField_ACCESSORS(jfloat, Float)
JniField_ACCESSORS(jdouble, Double)

#undef JniField_ACCESSORS

*/
