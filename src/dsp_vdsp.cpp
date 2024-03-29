//
//  dsp_vDSP.cpp
//  Kraken Engine / Siren
//
//  Copyright 2024 Kearwood Gilbert. All rights reserved.
//  
//  Redistribution and use in source and binary forms, with or without modification, are
//  permitted provided that the following conditions are met:
//  
//  1. Redistributions of source code must retain the above copyright notice, this list of
//  conditions and the following disclaimer.
//  
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//  of conditions and the following disclaimer in the documentation and/or other materials
//  provided with the distribution.
//  
//  THIS SOFTWARE IS PROVIDED BY KEARWOOD GILBERT ''AS IS'' AND ANY EXPRESS OR IMPLIED
//  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL KEARWOOD GILBERT OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
//  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  
//  The views and conclusions contained in the software and documentation are those of the
//  authors and should not be interpreted as representing official policies, either expressed
//  or implied, of Kearwood Gilbert.
//

#include "../include/dsp.h"

#ifdef SIREN_DSP_APPLE_VDSP

#include <Accelerate/Accelerate.h>

namespace siren {
namespace dsp {

FFTWorkspace::FFTWorkspace()
{
  setup = nullptr;
}

FFTWorkspace::~FFTWorkspace()
{
  destroy();
}

void FFTWorkspace::create(size_t length)
{
  setup = vDSP_create_fftsetup(length, kFFTRadix2);
}

void FFTWorkspace::destroy()
{
  if (setup) {
    vDSP_destroy_fftsetup(setup);
    setup = nullptr;
  }
}

void FFTForward(const FFTWorkspace& workspace, SplitComplex* src, size_t count)
{
  vDSP_fft_zip(workspace.setup, src, 1, count, kFFTDirection_Forward);
}

void FFTInverse(const FFTWorkspace& workspace, SplitComplex* src, size_t count)
{
  vDSP_fft_zip(workspace.setup, src, 1, count, kFFTDirection_Inverse);
}

void Int16ToFloat(const short* src, size_t srcStride, float* dest, size_t destStride, size_t count)
{
  vDSP_vflt16(src, srcStride, dest, destStride, count);
}

void Scale(float* buffer, float scale, size_t count)
{
  vDSP_vsmul(buffer, 1, &scale, buffer, 1, count);
}

void ScaleCopy(const float* src, float scale, float* dest, size_t count)
{
  vDSP_vsmul(src, 1, &scale, dest, 1, count);
}

void ScaleCopy(const SplitComplex* src, float scale, SplitComplex* dest, size_t count)
{
  ScaleCopy(src->realp, scale, dest->realp, count);
  ScaleCopy(src->imagp, scale, dest->imagp, count);
}

void ScaleRamp(float* buffer, float scaleStart, float scaleStep, size_t count)
{
  vDSP_vrampmul(buffer, 1, &scaleStart, &scaleStep, buffer, 1, count);
}

void Accumulate(float* buffer, size_t bufferStride, const float* buffer2, size_t buffer2Stride, size_t count)
{
  vDSP_vadd(buffer, bufferStride, buffer2, buffer2Stride, buffer, bufferStride, count);
}

void Accumulate(SplitComplex* buffer, const SplitComplex* buffer2, size_t count)
{
  vDSP_zvadd(buffer2, 1, buffer, 1, buffer, 1, count);
}


void Multiply(const SplitComplex* a, const SplitComplex* b, SplitComplex* c, size_t count)
{
  vDSP_zvmul(a, 1, b, 1, c, 1, count, 1);
}

} // namespace dsp
} // namespace siren

#endif // SIREN_DSP_APPLE_VDSP
