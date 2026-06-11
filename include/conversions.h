#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <string_view>
#include <cstdint>
#include <unordered_set>
#include <cstring>
#include <vector>

constexpr uint32_t fnv1a(std::string_view str) {
    uint32_t hash = 2166136261u;
    for (char c : str) {
        hash ^= static_cast<uint32_t>(c);
        hash *= 16777619u;
    }
    return hash;
}

constexpr uint64_t combine_hashes(uint32_t from, uint32_t to) {
    return (static_cast<uint64_t>(from) << 32) | to;
}

struct FORMAT {
    static constexpr uint32_t UNKNOWN = 0;

    enum class VIDEO : uint32_t {
        _3G2     = fnv1a("3g2"),    _3GP     = fnv1a("3gp"),    AAF      = fnv1a("aaf"),
        ASF      = fnv1a("asf"),    AVCHD    = fnv1a("avchd"),  AVI      = fnv1a("avi"),
        CAVS     = fnv1a("cavs"),   DIVX     = fnv1a("divx"),   DV       = fnv1a("dv"),
        F4V      = fnv1a("f4v"),    FLV      = fnv1a("flv"),    M2TS     = fnv1a("m2ts"),
        M2V      = fnv1a("m2v"),    M4V      = fnv1a("m4v"),    MJPEG    = fnv1a("mjpeg"),
        MKV      = fnv1a("mkv"),    MOD      = fnv1a("mod"),    MOV      = fnv1a("mov"),
        MP4      = fnv1a("mp4"),    MPEG     = fnv1a("mpeg"),   MPEG_2   = fnv1a("mpeg-2"),
        MPG      = fnv1a("mpg"),    MTS      = fnv1a("mts"),    MXF      = fnv1a("mxf"),
        OGV      = fnv1a("ogv"),    RM       = fnv1a("rm"),     RMVB     = fnv1a("rmvb"),
        SWF      = fnv1a("swf"),    TOD      = fnv1a("tod"),    TS       = fnv1a("ts"),
        VOB      = fnv1a("vob"),    WEBM     = fnv1a("webm"),   WMV      = fnv1a("wmv"),
        WTV      = fnv1a("wtv"),    XVID     = fnv1a("xvid")
    };

    enum class AUDIO : uint32_t {
        _8SVX    = fnv1a("8svx"),   AAC      = fnv1a("aac"),    AC3      = fnv1a("ac3"),
        AIFF     = fnv1a("aiff"),   AMB      = fnv1a("amb"),    AMR      = fnv1a("amr"),
        APE      = fnv1a("ape"),    AU       = fnv1a("au"),     AVR      = fnv1a("avr"),
        CAF      = fnv1a("caf"),    CDDA     = fnv1a("cdda"),   CVS      = fnv1a("cvs"),
        CVSD     = fnv1a("cvsd"),   CVU      = fnv1a("cvu"),    DSS      = fnv1a("dss"),
        DTS      = fnv1a("dts"),    DVMS     = fnv1a("dvms"),   FAP      = fnv1a("fap"),
        FLAC     = fnv1a("flac"),   FSSD     = fnv1a("fssd"),   GSM      = fnv1a("gsm"),
        GSRT     = fnv1a("gsrt"),   HCOM     = fnv1a("hcom"),   HTK      = fnv1a("htk"),
        IMA      = fnv1a("ima"),    IRCAM    = fnv1a("ircam"),  M4A      = fnv1a("m4a"),
        M4R      = fnv1a("m4r"),    MAUD     = fnv1a("maud"),   MP2      = fnv1a("mp2"),
        MP3      = fnv1a("mp3"),    NIST     = fnv1a("nist"),   OGA      = fnv1a("oga"),
        OGG      = fnv1a("ogg"),    OPUS     = fnv1a("opus"),   PAF      = fnv1a("paf"),
        PRC      = fnv1a("prc"),    PVF      = fnv1a("pvf"),    RA       = fnv1a("ra"),
        SD2      = fnv1a("sd2"),    SHN      = fnv1a("shn"),    SLN      = fnv1a("sln"),
        SMP      = fnv1a("smp"),    SND      = fnv1a("snd"),    SNDR     = fnv1a("sndr"),
        SNDT     = fnv1a("sndt"),   SOU      = fnv1a("sou"),    SPH      = fnv1a("sph"),
        SPX      = fnv1a("spx"),    TAK      = fnv1a("tak"),    TTA      = fnv1a("tta"),
        TXW      = fnv1a("txw"),    VMS      = fnv1a("vms"),    VOC      = fnv1a("voc"),
        VOX      = fnv1a("vox"),    VQF      = fnv1a("vqf"),    W64      = fnv1a("w64"),
        WAV      = fnv1a("wav"),    WMA      = fnv1a("wma"),    WV       = fnv1a("wv"),
        WVE      = fnv1a("wve"),    XA       = fnv1a("xa")
    };

    enum class IMAGE : uint32_t {
        _3FR     = fnv1a("3fr"),    ARW      = fnv1a("arw"),    AVIF     = fnv1a("avif"),
        BMP      = fnv1a("bmp"),    CR2      = fnv1a("cr2"),    CRW      = fnv1a("crw"),
        CUR      = fnv1a("cur"),    DCM      = fnv1a("dcm"),    DCR      = fnv1a("dcr"),
        DDS      = fnv1a("dds"),    DNG      = fnv1a("dng"),    ERF      = fnv1a("erf"),
        EXR      = fnv1a("exr"),    FAX      = fnv1a("fax"),    FTS      = fnv1a("fts"),
        G3       = fnv1a("g3"),     G4       = fnv1a("g4"),     GIF      = fnv1a("gif"),
        GV       = fnv1a("gv"),     HDR      = fnv1a("hdr"),    HEIC     = fnv1a("heic"),
        HEIF     = fnv1a("heif"),   HRZ      = fnv1a("hrz"),    ICO      = fnv1a("ico"),
        IIQ      = fnv1a("iiq"),    IPL      = fnv1a("ipl"),    JBG      = fnv1a("jbg"),
        JBIG     = fnv1a("jbig"),   JFI      = fnv1a("jfi"),    JFIF     = fnv1a("jfif"),
        JIF      = fnv1a("jif"),    JNX      = fnv1a("jnx"),    JP2      = fnv1a("jp2"),
        JPE      = fnv1a("jpe"),    JPEG     = fnv1a("jpeg"),   JPG      = fnv1a("jpg"),
        JPS      = fnv1a("jps"),    K25      = fnv1a("k25"),    KDC      = fnv1a("kdc"),
        MAC      = fnv1a("mac"),    MAP      = fnv1a("map"),    MEF      = fnv1a("mef"),
        MNG      = fnv1a("mng"),    MRW      = fnv1a("mrw"),    MTV      = fnv1a("mtv"),
        NEF      = fnv1a("nef"),    NRW      = fnv1a("nrw"),    ORF      = fnv1a("orf"),
        OTB      = fnv1a("otb"),    PAL      = fnv1a("pal"),    PALM     = fnv1a("palm"),
        PAM      = fnv1a("pam"),    PBM      = fnv1a("pbm"),    PCD      = fnv1a("pcd"),
        PCT      = fnv1a("pct"),    PCX      = fnv1a("pcx"),    PDB      = fnv1a("pdb"),
        PEF      = fnv1a("pef"),    PES      = fnv1a("pes"),    PFM      = fnv1a("pfm"),
        PGM      = fnv1a("pgm"),    PGX      = fnv1a("pgx"),    PICON    = fnv1a("picon"),
        PICT     = fnv1a("pict"),   PIX      = fnv1a("pix"),    PLASMA   = fnv1a("plasma"),
        PNG      = fnv1a("png"),    PNM      = fnv1a("pnm"),    PPM      = fnv1a("ppm"),
        PSD      = fnv1a("psd"),    PWP      = fnv1a("pwp"),    RAF      = fnv1a("raf"),
        RAS      = fnv1a("ras"),    RGB      = fnv1a("rgb"),    RGBA     = fnv1a("rgba"),
        RGBO     = fnv1a("rgbo"),   RGF      = fnv1a("rgf"),    RLA      = fnv1a("rla"),
        RLE      = fnv1a("rle"),    RW2      = fnv1a("rw2"),    SCT      = fnv1a("sct"),
        SFW      = fnv1a("sfw"),    SGI      = fnv1a("sgi"),    SIX      = fnv1a("six"),
        SIXEL    = fnv1a("sixel"),  SR2      = fnv1a("sr2"),    SRF      = fnv1a("srf"),
        SUN      = fnv1a("sun"),    SVG      = fnv1a("svg"),    TGA      = fnv1a("tga"),
        TIFF     = fnv1a("tiff"),   TIM      = fnv1a("tim"),    TM2      = fnv1a("tm2"),
        UYVY     = fnv1a("uyvy"),   VIFF     = fnv1a("viff"),   VIPS     = fnv1a("vips"),
        WBMP     = fnv1a("wbmp"),   WEBP     = fnv1a("webp"),   WMZ      = fnv1a("wmz"),
        WPG      = fnv1a("wpg"),    X3F      = fnv1a("x3f"),    XBM      = fnv1a("xbm"),
        XC       = fnv1a("xc"),     XCF      = fnv1a("xcf"),    XPM      = fnv1a("xpm"),
        XV       = fnv1a("xv"),     XWD      = fnv1a("xwd"),    YUV      = fnv1a("yuv")
    };
    
    enum class DOCUMENT : uint32_t {
        ABW      = fnv1a("abw"),    AW       = fnv1a("aw"),     CSV      = fnv1a("csv"),
        DBK      = fnv1a("dbk"),    DJVU     = fnv1a("djvu"),   DOC      = fnv1a("doc"),
        DOCM     = fnv1a("docm"),   DOCX     = fnv1a("docx"),   DOT      = fnv1a("dot"),
        DOTM     = fnv1a("dotm"),   DOTX     = fnv1a("dotx"),   HTML     = fnv1a("html"),
        KWD      = fnv1a("kwd"),    ODT      = fnv1a("odt"),    OXPS     = fnv1a("oxps"),
        PDF      = fnv1a("pdf"),    RTF      = fnv1a("rtf"),    SXW      = fnv1a("sxw"),
        TXT      = fnv1a("txt"),    WPS      = fnv1a("wps"),    XLS      = fnv1a("xls"),
        XLSX     = fnv1a("xlsx"),   XPS      = fnv1a("xps")
    };

    enum class ARCHIVE : uint32_t {
        _7Z      = fnv1a("7z"),     ACE      = fnv1a("ace"),    ALZ      = fnv1a("alz"),
        ARC      = fnv1a("arc"),    ARJ      = fnv1a("arj"),    CAB      = fnv1a("cab"),
        CPIO     = fnv1a("cpio"),   DEB      = fnv1a("deb"),    JAR      = fnv1a("jar"),
        LHA      = fnv1a("lha"),    RAR      = fnv1a("rar"),    RPM      = fnv1a("rpm"),
        TAR      = fnv1a("tar"),    TAR_7Z   = fnv1a("tar.7z"), TAR_BZ   = fnv1a("tar.bz"),
        TAR_LZ   = fnv1a("tar.lz"), TAR_LZMA = fnv1a("tar.lzma"),TAR_LZO  = fnv1a("tar.lzo"),
        TAR_XZ   = fnv1a("tar.xz"), TAR_Z    = fnv1a("tar.z"),  TBZ2     = fnv1a("tbz2"),
        TGZ      = fnv1a("tgz"),    ZIP      = fnv1a("zip")
    };

    enum class PRESENTATION : uint32_t {
        KEY      = fnv1a("key"),     ODP      = fnv1a("odp"),    POT      = fnv1a("pot"),
        POTX     = fnv1a("potx"),    PPS      = fnv1a("pps"),    PPSX     = fnv1a("ppsx"),
        PPT      = fnv1a("ppt"),     PPTX     = fnv1a("pptx")
    };

    enum class EBOOK : uint32_t {
        AZW3     = fnv1a("azw3"),    DJVU     = fnv1a("djvu"),   EPUB     = fnv1a("epub"),
        FB2      = fnv1a("fb2"),     LRF      = fnv1a("lrf"),    MOBI     = fnv1a("mobi")
    };

    enum class FONT : uint32_t {
        AFM      = fnv1a("afm"),     BIN      = fnv1a("bin"),    CFF      = fnv1a("cff"),
        CID      = fnv1a("cid"),     DFONT    = fnv1a("dfont"),   OTF      = fnv1a("otf"),
        PFA      = fnv1a("pfa"),     PFB      = fnv1a("pfb"),    PS       = fnv1a("ps"),
        PT3      = fnv1a("pt3"),     SFD      = fnv1a("sfd"),    _T11     = fnv1a("t11"),
        _T42     = fnv1a("t42"),     TTF      = fnv1a("ttf"),    UFO      = fnv1a("ufo"),
        WOFF     = fnv1a("woff")
    };

    private:
        struct FormatMapping {
            uint32_t hash;
            std::string_view name;
        };

        static constexpr FormatMapping formats[] = {
            // VIDEO
            { static_cast<uint32_t>(VIDEO::_3G2), "3g2" },
            { static_cast<uint32_t>(VIDEO::_3GP), "3gp" },
            { static_cast<uint32_t>(VIDEO::AAF), "aaf" },
            { static_cast<uint32_t>(VIDEO::ASF), "asf" },
            { static_cast<uint32_t>(VIDEO::AVCHD), "avchd" },
            { static_cast<uint32_t>(VIDEO::AVI), "avi" },
            { static_cast<uint32_t>(VIDEO::CAVS), "cavs" },
            { static_cast<uint32_t>(VIDEO::DIVX), "divx" },
            { static_cast<uint32_t>(VIDEO::DV), "dv" },
            { static_cast<uint32_t>(VIDEO::F4V), "f4v" },
            { static_cast<uint32_t>(VIDEO::FLV), "flv" },
            { static_cast<uint32_t>(VIDEO::M2TS), "m2ts" },
            { static_cast<uint32_t>(VIDEO::M2V), "m2v" },
            { static_cast<uint32_t>(VIDEO::M4V), "m4v" },
            { static_cast<uint32_t>(VIDEO::MJPEG), "mjpeg" },
            { static_cast<uint32_t>(VIDEO::MKV), "mkv" },
            { static_cast<uint32_t>(VIDEO::MOD), "mod" },
            { static_cast<uint32_t>(VIDEO::MOV), "mov" },
            { static_cast<uint32_t>(VIDEO::MP4), "mp4" },
            { static_cast<uint32_t>(VIDEO::MPEG), "mpeg" },
            { static_cast<uint32_t>(VIDEO::MPEG_2), "mpeg-2" },
            { static_cast<uint32_t>(VIDEO::MPG), "mpg" },
            { static_cast<uint32_t>(VIDEO::MTS), "mts" },
            { static_cast<uint32_t>(VIDEO::MXF), "mxf" },
            { static_cast<uint32_t>(VIDEO::OGV), "ogv" },
            { static_cast<uint32_t>(VIDEO::RM), "rm" },
            { static_cast<uint32_t>(VIDEO::RMVB), "rmvb" },
            { static_cast<uint32_t>(VIDEO::SWF), "swf" },
            { static_cast<uint32_t>(VIDEO::TOD), "tod" },
            { static_cast<uint32_t>(VIDEO::TS), "ts" },
            { static_cast<uint32_t>(VIDEO::VOB), "vob" },
            { static_cast<uint32_t>(VIDEO::WEBM), "webm" },
            { static_cast<uint32_t>(VIDEO::WMV), "wmv" },
            { static_cast<uint32_t>(VIDEO::WTV), "wtv" },
            { static_cast<uint32_t>(VIDEO::XVID), "xvid" },

            // AUDIO
            { static_cast<uint32_t>(AUDIO::_8SVX), "8svx" },
            { static_cast<uint32_t>(AUDIO::AAC), "aac" },
            { static_cast<uint32_t>(AUDIO::AC3), "ac3" },
            { static_cast<uint32_t>(AUDIO::AIFF), "aiff" },
            { static_cast<uint32_t>(AUDIO::AMB), "amb" },
            { static_cast<uint32_t>(AUDIO::AMR), "amr" },
            { static_cast<uint32_t>(AUDIO::APE), "ape" },
            { static_cast<uint32_t>(AUDIO::AU), "au" },
            { static_cast<uint32_t>(AUDIO::AVR), "avr" },
            { static_cast<uint32_t>(AUDIO::CAF), "caf" },
            { static_cast<uint32_t>(AUDIO::CDDA), "cdda" },
            { static_cast<uint32_t>(AUDIO::CVS), "cvs" },
            { static_cast<uint32_t>(AUDIO::CVSD), "cvsd" },
            { static_cast<uint32_t>(AUDIO::CVU), "cvu" },
            { static_cast<uint32_t>(AUDIO::DSS), "dss" },
            { static_cast<uint32_t>(AUDIO::DTS), "dts" },
            { static_cast<uint32_t>(AUDIO::DVMS), "dvms" },
            { static_cast<uint32_t>(AUDIO::FAP), "fap" },
            { static_cast<uint32_t>(AUDIO::FLAC), "flac" },
            { static_cast<uint32_t>(AUDIO::FSSD), "fssd" },
            { static_cast<uint32_t>(AUDIO::GSM), "gsm" },
            { static_cast<uint32_t>(AUDIO::GSRT), "gsrt" },
            { static_cast<uint32_t>(AUDIO::HCOM), "hcom" },
            { static_cast<uint32_t>(AUDIO::HTK), "htk" },
            { static_cast<uint32_t>(AUDIO::IMA), "ima" },
            { static_cast<uint32_t>(AUDIO::IRCAM), "ircam" },
            { static_cast<uint32_t>(AUDIO::M4A), "m4a" },
            { static_cast<uint32_t>(AUDIO::M4R), "m4r" },
            { static_cast<uint32_t>(AUDIO::MAUD), "maud" },
            { static_cast<uint32_t>(AUDIO::MP2), "mp2" },
            { static_cast<uint32_t>(AUDIO::MP3), "mp3" },
            { static_cast<uint32_t>(AUDIO::NIST), "nist" },
            { static_cast<uint32_t>(AUDIO::OGA), "oga" },
            { static_cast<uint32_t>(AUDIO::OGG), "ogg" },
            { static_cast<uint32_t>(AUDIO::OPUS), "opus" },
            { static_cast<uint32_t>(AUDIO::PAF), "paf" },
            { static_cast<uint32_t>(AUDIO::PRC), "prc" },
            { static_cast<uint32_t>(AUDIO::PVF), "pvf" },
            { static_cast<uint32_t>(AUDIO::RA), "ra" },
            { static_cast<uint32_t>(AUDIO::SD2), "sd2" },
            { static_cast<uint32_t>(AUDIO::SHN), "shn" },
            { static_cast<uint32_t>(AUDIO::SLN), "sln" },
            { static_cast<uint32_t>(AUDIO::SMP), "smp" },
            { static_cast<uint32_t>(AUDIO::SND), "snd" },
            { static_cast<uint32_t>(AUDIO::SNDR), "sndr" },
            { static_cast<uint32_t>(AUDIO::SNDT), "sndt" },
            { static_cast<uint32_t>(AUDIO::SOU), "sou" },
            { static_cast<uint32_t>(AUDIO::SPH), "sph" },
            { static_cast<uint32_t>(AUDIO::SPX), "spx" },
            { static_cast<uint32_t>(AUDIO::TAK), "tak" },
            { static_cast<uint32_t>(AUDIO::TTA), "tta" },
            { static_cast<uint32_t>(AUDIO::TXW), "txw" },
            { static_cast<uint32_t>(AUDIO::VMS), "vms" },
            { static_cast<uint32_t>(AUDIO::VOC), "voc" },
            { static_cast<uint32_t>(AUDIO::VOX), "vox" },
            { static_cast<uint32_t>(AUDIO::VQF), "vqf" },
            { static_cast<uint32_t>(AUDIO::W64), "w64" },
            { static_cast<uint32_t>(AUDIO::WAV), "wav" },
            { static_cast<uint32_t>(AUDIO::WMA), "wma" },
            { static_cast<uint32_t>(AUDIO::WV), "wv" },
            { static_cast<uint32_t>(AUDIO::WVE), "wve" },
            { static_cast<uint32_t>(AUDIO::XA), "xa" },

            // IMAGE
            { static_cast<uint32_t>(IMAGE::_3FR), "3fr" },
            { static_cast<uint32_t>(IMAGE::ARW), "arw" },
            { static_cast<uint32_t>(IMAGE::AVIF), "avif" },
            { static_cast<uint32_t>(IMAGE::BMP), "bmp" },
            { static_cast<uint32_t>(IMAGE::CR2), "cr2" },
            { static_cast<uint32_t>(IMAGE::CRW), "crw" },
            { static_cast<uint32_t>(IMAGE::CUR), "cur" },
            { static_cast<uint32_t>(IMAGE::DCM), "dcm" },
            { static_cast<uint32_t>(IMAGE::DCR), "dcr" },
            { static_cast<uint32_t>(IMAGE::DDS), "dds" },
            { static_cast<uint32_t>(IMAGE::DNG), "dng" },
            { static_cast<uint32_t>(IMAGE::ERF), "erf" },
            { static_cast<uint32_t>(IMAGE::EXR), "exr" },
            { static_cast<uint32_t>(IMAGE::FAX), "fax" },
            { static_cast<uint32_t>(IMAGE::FTS), "fts" },
            { static_cast<uint32_t>(IMAGE::G3), "g3" },
            { static_cast<uint32_t>(IMAGE::G4), "g4" },
            { static_cast<uint32_t>(IMAGE::GIF), "gif" },
            { static_cast<uint32_t>(IMAGE::GV), "gv" },
            { static_cast<uint32_t>(IMAGE::HDR), "hdr" },
            { static_cast<uint32_t>(IMAGE::HEIC), "heic" },
            { static_cast<uint32_t>(IMAGE::HEIF), "heif" },
            { static_cast<uint32_t>(IMAGE::HRZ), "hrz" },
            { static_cast<uint32_t>(IMAGE::ICO), "ico" },
            { static_cast<uint32_t>(IMAGE::IIQ), "iiq" },
            { static_cast<uint32_t>(IMAGE::IPL), "ipl" },
            { static_cast<uint32_t>(IMAGE::JBG), "jbg" },
            { static_cast<uint32_t>(IMAGE::JBIG), "jbig" },
            { static_cast<uint32_t>(IMAGE::JFI), "jfi" },
            { static_cast<uint32_t>(IMAGE::JFIF), "jfif" },
            { static_cast<uint32_t>(IMAGE::JIF), "jif" },
            { static_cast<uint32_t>(IMAGE::JNX), "jnx" },
            { static_cast<uint32_t>(IMAGE::JP2), "jp2" },
            { static_cast<uint32_t>(IMAGE::JPE), "jpe" },
            { static_cast<uint32_t>(IMAGE::JPEG), "jpeg" },
            { static_cast<uint32_t>(IMAGE::JPG), "jpg" },
            { static_cast<uint32_t>(IMAGE::JPS), "jps" },
            { static_cast<uint32_t>(IMAGE::K25), "k25" },
            { static_cast<uint32_t>(IMAGE::KDC), "kdc" },
            { static_cast<uint32_t>(IMAGE::MAC), "mac" },
            { static_cast<uint32_t>(IMAGE::MAP), "map" },
            { static_cast<uint32_t>(IMAGE::MEF), "mef" },
            { static_cast<uint32_t>(IMAGE::MNG), "mng" },
            { static_cast<uint32_t>(IMAGE::MRW), "mrw" },
            { static_cast<uint32_t>(IMAGE::MTV), "mtv" },
            { static_cast<uint32_t>(IMAGE::NEF), "nef" },
            { static_cast<uint32_t>(IMAGE::NRW), "nrw" },
            { static_cast<uint32_t>(IMAGE::ORF), "orf" },
            { static_cast<uint32_t>(IMAGE::OTB), "otb" },
            { static_cast<uint32_t>(IMAGE::PAL), "pal" },
            { static_cast<uint32_t>(IMAGE::PALM), "palm" },
            { static_cast<uint32_t>(IMAGE::PAM), "pam" },
            { static_cast<uint32_t>(IMAGE::PBM), "pbm" },
            { static_cast<uint32_t>(IMAGE::PCD), "pcd" },
            { static_cast<uint32_t>(IMAGE::PCT), "pct" },
            { static_cast<uint32_t>(IMAGE::PCX), "pcx" },
            { static_cast<uint32_t>(IMAGE::PDB), "pdb" },
            { static_cast<uint32_t>(IMAGE::PEF), "pef" },
            { static_cast<uint32_t>(IMAGE::PES), "pes" },
            { static_cast<uint32_t>(IMAGE::PFM), "pfm" },
            { static_cast<uint32_t>(IMAGE::PGM), "pgm" },
            { static_cast<uint32_t>(IMAGE::PGX), "pgx" },
            { static_cast<uint32_t>(IMAGE::PICON), "picon" },
            { static_cast<uint32_t>(IMAGE::PICT), "pict" },
            { static_cast<uint32_t>(IMAGE::PIX), "pix" },
            { static_cast<uint32_t>(IMAGE::PLASMA), "plasma" },
            { static_cast<uint32_t>(IMAGE::PNG), "png" },
            { static_cast<uint32_t>(IMAGE::PNM), "pnm" },
            { static_cast<uint32_t>(IMAGE::PPM), "ppm" },
            { static_cast<uint32_t>(IMAGE::PSD), "psd" },
            { static_cast<uint32_t>(IMAGE::PWP), "pwp" },
            { static_cast<uint32_t>(IMAGE::RAF), "raf" },
            { static_cast<uint32_t>(IMAGE::RAS), "ras" },
            { static_cast<uint32_t>(IMAGE::RGB), "rgb" },
            { static_cast<uint32_t>(IMAGE::RGBA), "rgba" },
            { static_cast<uint32_t>(IMAGE::RGBO), "rgbo" },
            { static_cast<uint32_t>(IMAGE::RGF), "rgf" },
            { static_cast<uint32_t>(IMAGE::RLA), "rla" },
            { static_cast<uint32_t>(IMAGE::RLE), "rle" },
            { static_cast<uint32_t>(IMAGE::RW2), "rw2" },
            { static_cast<uint32_t>(IMAGE::SCT), "sct" },
            { static_cast<uint32_t>(IMAGE::SFW), "sfw" },
            { static_cast<uint32_t>(IMAGE::SGI), "sgi" },
            { static_cast<uint32_t>(IMAGE::SIX), "six" },
            { static_cast<uint32_t>(IMAGE::SIXEL), "sixel" },
            { static_cast<uint32_t>(IMAGE::SR2), "sr2" },
            { static_cast<uint32_t>(IMAGE::SRF), "srf" },
            { static_cast<uint32_t>(IMAGE::SUN), "sun" },
            { static_cast<uint32_t>(IMAGE::SVG), "svg" },
            { static_cast<uint32_t>(IMAGE::TGA), "tga" },
            { static_cast<uint32_t>(IMAGE::TIFF), "tiff" },
            { static_cast<uint32_t>(IMAGE::TIM), "tim" },
            { static_cast<uint32_t>(IMAGE::TM2), "tm2" },
            { static_cast<uint32_t>(IMAGE::UYVY), "uyvy" },
            { static_cast<uint32_t>(IMAGE::VIFF), "viff" },
            { static_cast<uint32_t>(IMAGE::VIPS), "vips" },
            { static_cast<uint32_t>(IMAGE::WBMP), "wbmp" },
            { static_cast<uint32_t>(IMAGE::WEBP), "webp" },
            { static_cast<uint32_t>(IMAGE::WMZ), "wmz" },
            { static_cast<uint32_t>(IMAGE::WPG), "wpg" },
            { static_cast<uint32_t>(IMAGE::X3F), "x3f" },
            { static_cast<uint32_t>(IMAGE::XBM), "xbm" },
            { static_cast<uint32_t>(IMAGE::XC), "xc" },
            { static_cast<uint32_t>(IMAGE::XCF), "xcf" },
            { static_cast<uint32_t>(IMAGE::XPM), "xpm" },
            { static_cast<uint32_t>(IMAGE::XV), "xv" },
            { static_cast<uint32_t>(IMAGE::XWD), "xwd" },
            { static_cast<uint32_t>(IMAGE::YUV), "yuv" },

            // DOCUMENT
            { static_cast<uint32_t>(DOCUMENT::ABW), "abw" },
            { static_cast<uint32_t>(DOCUMENT::AW), "aw" },
            { static_cast<uint32_t>(DOCUMENT::CSV), "csv" },
            { static_cast<uint32_t>(DOCUMENT::DBK), "dbk" },
            { static_cast<uint32_t>(DOCUMENT::DJVU), "djvu" },
            { static_cast<uint32_t>(DOCUMENT::DOC), "doc" },
            { static_cast<uint32_t>(DOCUMENT::DOCM), "docm" },
            { static_cast<uint32_t>(DOCUMENT::DOCX), "docx" },
            { static_cast<uint32_t>(DOCUMENT::DOT), "dot" },
            { static_cast<uint32_t>(DOCUMENT::DOTM), "dotm" },
            { static_cast<uint32_t>(DOCUMENT::DOTX), "dotx" },
            { static_cast<uint32_t>(DOCUMENT::HTML), "html" },
            { static_cast<uint32_t>(DOCUMENT::KWD), "kwd" },
            { static_cast<uint32_t>(DOCUMENT::ODT), "odt" },
            { static_cast<uint32_t>(DOCUMENT::OXPS), "oxps" },
            { static_cast<uint32_t>(DOCUMENT::PDF), "pdf" },
            { static_cast<uint32_t>(DOCUMENT::RTF), "rtf" },
            { static_cast<uint32_t>(DOCUMENT::SXW), "sxw" },
            { static_cast<uint32_t>(DOCUMENT::TXT), "txt" },
            { static_cast<uint32_t>(DOCUMENT::WPS), "wps" },
            { static_cast<uint32_t>(DOCUMENT::XLS), "xls" },
            { static_cast<uint32_t>(DOCUMENT::XLSX), "xlsx" },
            { static_cast<uint32_t>(DOCUMENT::XPS), "xps" },

            // ARCHIVE
            { static_cast<uint32_t>(ARCHIVE::_7Z), "7z" },
            { static_cast<uint32_t>(ARCHIVE::ACE), "ace" },
            { static_cast<uint32_t>(ARCHIVE::ALZ), "alz" },
            { static_cast<uint32_t>(ARCHIVE::ARC), "arc" },
            { static_cast<uint32_t>(ARCHIVE::ARJ), "arj" },
            { static_cast<uint32_t>(ARCHIVE::CAB), "cab" },
            { static_cast<uint32_t>(ARCHIVE::CPIO), "cpio" },
            { static_cast<uint32_t>(ARCHIVE::DEB), "deb" },
            { static_cast<uint32_t>(ARCHIVE::JAR), "jar" },
            { static_cast<uint32_t>(ARCHIVE::LHA), "lha" },
            { static_cast<uint32_t>(ARCHIVE::RAR), "rar" },
            { static_cast<uint32_t>(ARCHIVE::RPM), "rpm" },
            { static_cast<uint32_t>(ARCHIVE::TAR), "tar" },
            { static_cast<uint32_t>(ARCHIVE::TAR_7Z), "tar.7z" },
            { static_cast<uint32_t>(ARCHIVE::TAR_BZ), "tar.bz" },
            { static_cast<uint32_t>(ARCHIVE::TAR_LZ), "tar.lz" },
            { static_cast<uint32_t>(ARCHIVE::TAR_LZMA), "tar.lzma" },
            { static_cast<uint32_t>(ARCHIVE::TAR_LZO), "tar.lzo" },
            { static_cast<uint32_t>(ARCHIVE::TAR_XZ), "tar.xz" },
            { static_cast<uint32_t>(ARCHIVE::TAR_Z), "tar.z" },
            { static_cast<uint32_t>(ARCHIVE::TBZ2), "tbz2" },
            { static_cast<uint32_t>(ARCHIVE::TGZ), "tgz" },
            { static_cast<uint32_t>(ARCHIVE::ZIP), "zip" },

            // PRESENTATION
            { static_cast<uint32_t>(PRESENTATION::KEY), "key" },
            { static_cast<uint32_t>(PRESENTATION::ODP), "odp" },
            { static_cast<uint32_t>(PRESENTATION::POT), "pot" },
            { static_cast<uint32_t>(PRESENTATION::POTX), "potx" },
            { static_cast<uint32_t>(PRESENTATION::PPS), "pps" },
            { static_cast<uint32_t>(PRESENTATION::PPSX), "ppsx" },
            { static_cast<uint32_t>(PRESENTATION::PPT), "ppt" },
            { static_cast<uint32_t>(PRESENTATION::PPTX), "pptx" },

            // EBOOK
            { static_cast<uint32_t>(EBOOK::AZW3), "azw3" },
            { static_cast<uint32_t>(EBOOK::DJVU), "djvu" },
            { static_cast<uint32_t>(EBOOK::EPUB), "epub" },
            { static_cast<uint32_t>(EBOOK::FB2), "fb2" },
            { static_cast<uint32_t>(EBOOK::LRF), "lrf" },
            { static_cast<uint32_t>(EBOOK::MOBI), "mobi" },

            // FONT
            { static_cast<uint32_t>(FONT::AFM), "afm" },
            { static_cast<uint32_t>(FONT::BIN), "bin" },
            { static_cast<uint32_t>(FONT::CFF), "cff" },
            { static_cast<uint32_t>(FONT::CID), "cid" },
            { static_cast<uint32_t>(FONT::DFONT), "dfont" },
            { static_cast<uint32_t>(FONT::OTF), "otf" },
            { static_cast<uint32_t>(FONT::PFA), "pfa" },
            { static_cast<uint32_t>(FONT::PFB), "pfb" },
            { static_cast<uint32_t>(FONT::PS), "ps" },
            { static_cast<uint32_t>(FONT::PT3), "pt3" },
            { static_cast<uint32_t>(FONT::SFD), "sfd" },
            { static_cast<uint32_t>(FONT::_T11), "t11" },
            { static_cast<uint32_t>(FONT::_T42), "t42" },
            { static_cast<uint32_t>(FONT::TTF), "ttf" },
            { static_cast<uint32_t>(FONT::UFO), "ufo" },
            { static_cast<uint32_t>(FONT::WOFF), "woff" }
        };

    public:
        template <typename T>
        static constexpr std::string_view toString(T format_enum) {
            uint32_t target_hash = static_cast<uint32_t>(format_enum);
            for (const auto& item : formats) {
                if (item.hash == target_hash) {
                    return item.name;
                }
            }
            return "unknown";
        }

        struct CategoryRange {
            const FormatMapping* begin_ptr = nullptr;
            const FormatMapping* end_ptr = nullptr;
        };

        template <typename T>
        static constexpr CategoryRange getCategoryRange(T first, T last) {
            const uint32_t first_target = static_cast<uint32_t>(first);
            const uint32_t last_target = static_cast<uint32_t>(last);

            CategoryRange range;

            for (size_t i = 0; i < std::size(formats); ++i) {
                if (formats[i].hash == first_target) {
                    range.begin_ptr = &formats[i];
                }
                if (formats[i].hash == last_target) {
                    range.end_ptr = &formats[i] + 1;
                    break;
                }
            }
            return range;
        }
        enum class Category {
            VIDEO, AUDIO, IMAGE, DOCUMENT, ARCHIVE, PRESENTATION, EBOOK, FONT, UNKNOWN
        };

        static constexpr Category getCategory(uint32_t hash) {
            for (const auto& item : formats) {
                if (item.hash != hash) continue;

                if (&item >= getCategoryRange(VIDEO::_3G2, VIDEO::XVID).begin_ptr && &item < getCategoryRange(VIDEO::_3G2, VIDEO::XVID).end_ptr) return Category::VIDEO;
                if (&item >= getCategoryRange(AUDIO::_8SVX, AUDIO::XA).begin_ptr && &item < getCategoryRange(AUDIO::_8SVX, AUDIO::XA).end_ptr) return Category::AUDIO;
                if (&item >= getCategoryRange(IMAGE::_3FR, IMAGE::YUV).begin_ptr && &item < getCategoryRange(IMAGE::_3FR, IMAGE::YUV).end_ptr) return Category::IMAGE;
                if (&item >= getCategoryRange(DOCUMENT::ABW, DOCUMENT::XPS).begin_ptr && &item < getCategoryRange(DOCUMENT::ABW, DOCUMENT::XPS).end_ptr) return Category::DOCUMENT;
                if (&item >= getCategoryRange(ARCHIVE::_7Z, ARCHIVE::ZIP).begin_ptr && &item < getCategoryRange(ARCHIVE::_7Z, ARCHIVE::ZIP).end_ptr) return Category::ARCHIVE;
                if (&item >= getCategoryRange(PRESENTATION::KEY, PRESENTATION::PPTX).begin_ptr && &item < getCategoryRange(PRESENTATION::KEY, PRESENTATION::PPTX).end_ptr) return Category::PRESENTATION;
                if (&item >= getCategoryRange(EBOOK::AZW3, EBOOK::MOBI).begin_ptr && &item < getCategoryRange(EBOOK::AZW3, EBOOK::MOBI).end_ptr) return Category::EBOOK;
                if (&item >= getCategoryRange(FONT::AFM, FONT::WOFF).begin_ptr && &item < getCategoryRange(FONT::AFM, FONT::WOFF).end_ptr) return Category::FONT;
            }
            return Category::UNKNOWN;
        }

        static constexpr bool checkIfPossible(std::string_view from, std::string_view to) {
            if (from.empty() || to.empty()) return false;
            uint32_t f_hash = fnv1a(from);
            uint32_t t_hash = fnv1a(to);
            if (f_hash == t_hash) return false;
            
            Category c_from = getCategory(f_hash);
            Category c_to = getCategory(t_hash);
            
            return c_from != Category::UNKNOWN && c_from == c_to;
        }
};

#endif