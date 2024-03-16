/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   abcbg_png;
    const int            abcbg_pngSize = 194680;

    extern const char*   mainknob_png;
    const int            mainknob_pngSize = 67593;

    extern const char*   ABS_png;
    const int            ABS_pngSize = 112366;

    extern const char*   CytomicBlue_png;
    const int            CytomicBlue_pngSize = 200915;

    extern const char*   finalbg_png;
    const int            finalbg_pngSize = 120461;

    extern const char*   finlog_png;
    const int            finlog_pngSize = 3800;

    extern const char*   finlogo121_png;
    const int            finlogo121_pngSize = 4253;

    extern const char*   bgjuce_png;
    const int            bgjuce_pngSize = 9283;

    extern const char*   mslog_png;
    const int            mslog_pngSize = 4410;

    extern const char*   mslogo_png;
    const int            mslogo_pngSize = 3421;

    extern const char*   knobsmall_png;
    const int            knobsmall_pngSize = 22320;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 11;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
