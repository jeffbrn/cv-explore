message("==================================================================================")
message(" Preparing FFTW")
message("==================================================================================")

include(FetchContent)

set(FFTW_LIB_URL https://www.fftw.org/fftw-3.3.10.tar.gz)
FetchContent_Declare(
    fftw
    URL ${FFTW_LIB_URL}
)

FetchContent_MakeAvailable(fftw)
