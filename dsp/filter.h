#include <cmath>
#include <complex>

struct Response 
{
    float mag[128];
};

struct Filter
{
    float a[2];
    float b[3];
};

Response getResponse(Filter F)
{
    using std::polar;
    const float samplerate = 48e3;
    Response res;
    const float low = 10;
    const float high = 20e3;
    const float l_ = log10(low), h_ = log10(high);
    for(int i=0; i<128; ++i) {
        float x = i/127.0;
        float f = powf(10.0f, l_+x*(h_-l_))/samplerate;
        std::complex<float> num = polar(F.b[0], 0.0f) + polar(F.b[1], float(f*M_PI)) + polar(F.b[2], float(2.0f*f*M_PI));
        std::complex<float> dem = polar(1.0f,0.0f) - polar(F.a[0], float(f*M_PI)) - polar(F.a[1], float(2.0*f*M_PI));
        res.mag[i] = 20*log10(abs(num/dem));
        printf("%d: %f == %f\n", i, f, res.mag[i]);
    }

    return res;
}

Response getLpf(float freq)
{
    const float samplerate = 48e3;
    const float tmp = expf(-2.0f*M_PI*freq/samplerate);
    Filter f;
    f.b[0]  = 1.0f - tmp;
    f.b[1]  = 0.0f;
    f.b[2]  = 0.0f;
    f.a[0]  = tmp;
    f.a[1]  = 0.0f;
    return getResponse(f);
}

Response getBpf(float freq, float Q)
{
    const float samplerate = 48e3;
    const float omega = 2 * M_PI * freq / samplerate;
    const float sn    = sinf(omega), cs = cosf(omega);
    const float alpha = sn / (2.0f * Q);
    const float tmp   = 1.0f + alpha;
    Filter f;
    f.b[0]  = alpha / tmp *sqrtf(Q + 1.0f);
    f.b[1]  = 0.0f;
    f.b[2]  = -alpha / tmp *sqrtf(Q + 1.0f);
    f.a[0]  = -2.0f * cs / tmp * -1.0f;
    f.a[1]  = (1.0f - alpha) / tmp * -1.0f;
    return getResponse(f);
}
