
#ifndef _OgreSTBICodecExport_H
#define _OgreSTBICodecExport_H

#ifdef CODEC_STBI_STATIC_DEFINE
#  define _OgreSTBICodecExport
#  define CODEC_STBI_NO_EXPORT
#else
#  ifndef _OgreSTBICodecExport
#    ifdef Codec_STBI_EXPORTS
        /* We are building this library */
#      define _OgreSTBICodecExport 
#    else
        /* We are using this library */
#      define _OgreSTBICodecExport 
#    endif
#  endif

#  ifndef CODEC_STBI_NO_EXPORT
#    define CODEC_STBI_NO_EXPORT 
#  endif
#endif

#ifndef CODEC_STBI_DEPRECATED
#  define CODEC_STBI_DEPRECATED __declspec(deprecated)
#endif

#ifndef CODEC_STBI_DEPRECATED_EXPORT
#  define CODEC_STBI_DEPRECATED_EXPORT _OgreSTBICodecExport CODEC_STBI_DEPRECATED
#endif

#ifndef CODEC_STBI_DEPRECATED_NO_EXPORT
#  define CODEC_STBI_DEPRECATED_NO_EXPORT CODEC_STBI_NO_EXPORT CODEC_STBI_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CODEC_STBI_NO_DEPRECATED
#    define CODEC_STBI_NO_DEPRECATED
#  endif
#endif

#endif /* _OgreSTBICodecExport_H */
