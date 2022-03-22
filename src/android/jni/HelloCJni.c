//
//  HelloCJni
//
#include <string.h>
#include <hello.h>
#include <HelloCJni.h>
#include <openssl/md5.h>

// Platform-specific C implementation to get current CPU architecture
JNIEXPORT jstring JNICALL Java_com_example_HelloCJni_getArch( JNIEnv* env, jclass thiz )
{
#if defined(__arm__)
  #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
   #define ABI "x86"
#elif defined(__x86_64__)
   #define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
   #define ABI "mips64"
#elif defined(__mips__)
   #define ABI "mips"
#elif defined(__aarch64__)
   #define ABI "arm64-v8a"
#else
   #define ABI "unknown"
#endif
    return (*env)->NewStringUTF(env, ABI);
}

JNIEXPORT jstring JNICALL Java_com_example_HelloCJni_getMd5(JNIEnv *env, jobject thiz, jstring pac_) {
    const char *pac = (*env)->GetStringUTFChars(env, pac_, NULL);

    unsigned  char c[MD5_DIGEST_LENGTH];
    int i;
    char dest[32]={0};
    FILE *fp;
    MD5_CTX mdContext;
    size_t bytes;
    unsigned char data[1024];

    fp = fopen(pac, "rb");
    MD5_Init (&mdContext);
    while(( bytes = fread(data, 1, 1024, fp)) != 0 )
        MD5_Update(&mdContext, data, bytes );
    MD5_Final(c, &mdContext );
    fclose(fp);

    for( i = 0 ; i < MD5_DIGEST_LENGTH ; i++ )
        sprintf(dest+i*2,"%02X",c[i]);

    (*env)->ReleaseStringUTFChars(env, pac_, pac);

    return (*env)->NewStringUTF(env, dest);
}

// Android JNI wrapper for cross-platform C implementation
JNIEXPORT jstring JNICALL Java_com_example_HelloCJni_hello( JNIEnv* env, jclass thiz, jstring j_input)
{
    // Call the cross-platform shared C function
    char* c_input = strdup((*env)->GetStringUTFChars(env, j_input, 0));
    char* output = c_hello(c_input);
    return (*env)->NewStringUTF(env, output);
}

// Android JNI wrapper for cross-platform C library
JNIEXPORT jstring JNICALL Java_com_example_HelloCJni_calculate( JNIEnv* env, jclass thiz, jint j_x, jint j_y)
{
    // Call the cross-platform shared C function
    int x = (int) j_x;
    int y = (int) j_y;
    int result = calculate(x, y);
    return result;
}

// Android JNI wrapper for cross-platform C library
JNIEXPORT jstring JNICALL Java_com_example_HelloCJni_crash( JNIEnv* env, jclass thiz)
{
    // Call the cross-platform shared C function
    int result = crash();
    return result;
}
