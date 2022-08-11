#include <hip/hip_runtime.h>

extern "C" __global__ __launch_bounds__(121) void _occa_UrstCubatureHex3D_0(const int Nelements,
                                                                                                          const double * __restrict__ D,
                                                                                                          const double * __restrict__ x,
                                                                                                          const double * __restrict__ y,
                                                                                                          const double * __restrict__ z,
                                                                                                          const double * __restrict__ cubInterpT,
                                                                                                          const double * __restrict__ cubW,
                                                                                                          const int offset,
                                                                                                          const int cubatureOffset,
                                                                                                          const double * __restrict__ U,
                                                                                                          const double * __restrict__ W,
                                                                                                          double * __restrict__ result) {
  {
    int element = 0 + blockIdx.x;
    __shared__ double s_cubInterpT[8][11];
    __shared__ double s_U[8][8];
    __shared__ double s_V[8][8];
    __shared__ double s_W[8][8];

    // reuse smem
    __shared__ double s_U1[8][11];
    __shared__ double s_V1[8][11];
    __shared__ double s_W1[8][11];

    //reuse smem
    double r_U[11], r_V[11], r_W[11];

    // reuse reg
    __shared__ double s_D[8][8];
    __shared__ double s_xre[8][8];
    __shared__ double s_xse[8][8];
    __shared__ double s_xte[8][8];
    __shared__ double s_yre[8][8];
    __shared__ double s_yse[8][8];
    __shared__ double s_yte[8][8];
    __shared__ double s_zre[8][8];
    __shared__ double s_zse[8][8];
    __shared__ double s_zte[8][8];
    __shared__ double s_cubyre[8][11];
    __shared__ double s_cubyse[8][11];
    __shared__ double s_cubyte[8][11];
    __shared__ double s_cubzre[8][11];
    __shared__ double s_cubzse[8][11];
    __shared__ double s_cubzte[8][11];
    double r_cubxre[11];
    double r_cubxse[11];
    double r_cubxte[11];
    double r_cubyre[11];
    double r_cubyse[11];
    double r_cubyte[11];
    double r_cubzre[11];
    double r_cubzse[11];
    double r_cubzte[11];
    __shared__ double s_cubW[11];
    {
      int j = 0 + threadIdx.y;
      {
        int i = 0 + threadIdx.x;
        const int id = i + j * 11;
        if (j == 0) {
          s_cubW[i] = cubW[i];
        }
        if (id < 8 * 11) {
          s_cubInterpT[j][i] = cubInterpT[id];
        }
#pragma unroll 11
        for (int k = 0; k < 11; ++k) {
          r_cubxre[k] = 0;
          r_cubxse[k] = 0;
          r_cubxte[k] = 0;
          r_cubyre[k] = 0;
          r_cubyse[k] = 0;
          r_cubyte[k] = 0;
          r_cubzre[k] = 0;
          r_cubzse[k] = 0;
          r_cubzte[k] = 0;
        }
      }
    }
    __syncthreads();

    //    #pragma unroll p_Nq
    for (int k = 0; k < 8; ++k) {
      {
        int j = 0 + threadIdx.y;
        {
          int i = 0 + threadIdx.x;
          if (i < 8 && j < 8) {
            s_D[j][i] = D[j * 8 + i];
            const int id = element * 512 + k * 8 * 8 + j * 8 + i;
            s_U[j][i] = x[id];
            s_V[j][i] = y[id];
            s_W[j][i] = z[id];
            if (k == 0) {
#pragma unroll 8
              for (int l = 0; l < 8; ++l) {
                const int other_id = element * 512 + l * 8 * 8 + j * 8 + i;
                r_U[l] = x[other_id];
                r_V[l] = y[other_id];
                r_W[l] = z[other_id];
              }
            }
          }
        }
      }
      ;
      __syncthreads();
      {
        int j = 0 + threadIdx.y;
        {
          int i = 0 + threadIdx.x;
          if (i < 8 && j < 8) {
            double xr = 0, yr = 0, zr = 0;
            double xs = 0, ys = 0, zs = 0;
            double xt = 0, yt = 0, zt = 0;
#pragma unroll 8
            for (int m = 0; m < 8; ++m) {
              const double Dim = s_D[i][m];
              const double Djm = s_D[j][m];
              const double Dkm = s_D[k][m];
              xr += Dim * s_U[j][m];
              xs += Djm * s_U[m][i];
              xt += Dkm * r_U[m];
              yr += Dim * s_V[j][m];
              ys += Djm * s_V[m][i];
              yt += Dkm * r_V[m];
              zr += Dim * s_W[j][m];
              zs += Djm * s_W[m][i];
              zt += Dkm * r_W[m];
            }
            // store results in shmem array
            s_xre[j][i] = xr;
            s_xse[j][i] = xs;
            s_xte[j][i] = xt;
            s_yre[j][i] = yr;
            s_yse[j][i] = ys;
            s_yte[j][i] = yt;
            s_zre[j][i] = zr;
            s_zse[j][i] = zs;
            s_zte[j][i] = zt;
          }
        }
      }
      ;
      __syncthreads();
      {
        int b = 0 + threadIdx.y;
        {
          int i = 0 + threadIdx.x;
          if (b < 8) {
            double xr1 = 0, xs1 = 0, xt1 = 0;
            double yr1 = 0, ys1 = 0, yt1 = 0;
            double zr1 = 0, zs1 = 0, zt1 = 0;
            for (int a = 0; a < 8; ++a) {
              double Iia = s_cubInterpT[a][i];
              xr1 += Iia * s_xre[b][a];
              xs1 += Iia * s_xse[b][a];
              xt1 += Iia * s_xte[b][a];
              yr1 += Iia * s_yre[b][a];
              ys1 += Iia * s_yse[b][a];
              yt1 += Iia * s_yte[b][a];
              zr1 += Iia * s_zre[b][a];
              zs1 += Iia * s_zse[b][a];
              zt1 += Iia * s_zte[b][a];
            }
            s_U1[b][i] = xr1;
            s_V1[b][i] = xs1;
            s_W1[b][i] = xt1;
            s_cubyre[b][i] = yr1;
            s_cubyse[b][i] = ys1;
            s_cubyte[b][i] = yt1;
            s_cubzre[b][i] = zr1;
            s_cubzse[b][i] = zs1;
            s_cubzte[b][i] = zt1;
          }
        }
      }
      ;
      __syncthreads();

      // interpolate in 's'
      {
        int j = 0 + threadIdx.y;
        {
          int i = 0 + threadIdx.x;
          double xr2 = 0, xs2 = 0, xt2 = 0;
          double yr2 = 0, ys2 = 0, yt2 = 0;
          double zr2 = 0, zs2 = 0, zt2 = 0;
          // interpolate in b
          for (int b = 0; b < 8; ++b) {
            double Ijb = s_cubInterpT[b][j];
            xr2 += Ijb * s_U1[b][i];
            xs2 += Ijb * s_V1[b][i];
            xt2 += Ijb * s_W1[b][i];
            yr2 += Ijb * s_cubyre[b][i];
            ys2 += Ijb * s_cubyse[b][i];
            yt2 += Ijb * s_cubyte[b][i];
            zr2 += Ijb * s_cubzre[b][i];
            zs2 += Ijb * s_cubzse[b][i];
            zt2 += Ijb * s_cubzte[b][i];
          }

          // interpolate in k progressively
#pragma unroll 11
          for (int c = 0; c < 11; ++c) {
            double Ick = s_cubInterpT[k][c];
            r_cubxre[c] += Ick * xr2;
            r_cubxse[c] += Ick * xs2;
            r_cubxte[c] += Ick * xt2;
            r_cubyre[c] += Ick * yr2;
            r_cubyse[c] += Ick * ys2;
            r_cubyte[c] += Ick * yt2;
            r_cubzre[c] += Ick * zr2;
            r_cubzse[c] += Ick * zs2;
            r_cubzte[c] += Ick * zt2;
          }
        }
      }
      ;
      __syncthreads();
    }
    for (int c = 0; c < 8; ++c) {
      __syncthreads();
      {
        int b = 0 + threadIdx.y;
        {
          int a = 0 + threadIdx.x;
          if (c == 0) {
#pragma unroll 11
            for (int k = 0; k < 11; ++k) {
              r_U[k] = 0.0;
              r_V[k] = 0.0;
              r_W[k] = 0.0;
            }
          }
          if (a < 8 && b < 8) {
            const int id = element * 512 + c * 8 * 8 + b * 8 + a;
            double Ue = U[id + 0 * offset];
            double Ve = U[id + 1 * offset];
            double We = U[id + 2 * offset];
            if (0) {
              Ue -= W[id + 0 * offset];
              Ve -= W[id + 1 * offset];
              We -= W[id + 2 * offset];
            }
            s_U[b][a] = Ue;
            s_V[b][a] = Ve;
            s_W[b][a] = We;
          }
        }
      }
      ;
      __syncthreads();

      // interpolate in 'r'
      {
        int b = 0 + threadIdx.y;
        {
          int i = 0 + threadIdx.x;
          if (b < 8) {
            double U1 = 0, V1 = 0, W1 = 0;
            for (int a = 0; a < 8; ++a) {
              double Iia = s_cubInterpT[a][i];
              U1 += Iia * s_U[b][a];
              V1 += Iia * s_V[b][a];
              W1 += Iia * s_W[b][a];
            }
            s_U1[b][i] = U1;
            s_V1[b][i] = V1;
            s_W1[b][i] = W1;
          }
        }
      }
      ;
      __syncthreads();

      // interpolate in 's'
      {
        int j = 0 + threadIdx.y;
        {
          int i = 0 + threadIdx.x;
          double U2 = 0, V2 = 0, W2 = 0;

          // interpolate in b
          for (int b = 0; b < 8; ++b) {
            double Ijb = s_cubInterpT[b][j];
            U2 += Ijb * s_U1[b][i];
            V2 += Ijb * s_V1[b][i];
            W2 += Ijb * s_W1[b][i];
          }

          // interpolate in c progressively
#pragma unroll 11
          for (int k = 0; k < 11; ++k) {
            double Ikc = s_cubInterpT[c][k];
            r_U[k] += Ikc * U2;
            r_V[k] += Ikc * V2;
            r_W[k] += Ikc * W2;
          }
        }
      }
      ;
    }
#pragma unroll 11
    for (int k = 0; k < 11; ++k) {
      __syncthreads();
      {
        int j = 0 + threadIdx.y;
        {
          int i = 0 + threadIdx.x;
          const double xr = r_cubxre[k], xs = r_cubxse[k], xt = r_cubxte[k];
          const double yr = r_cubyre[k], ys = r_cubyse[k], yt = r_cubyte[k];
          const double zr = r_cubzre[k], zs = r_cubzse[k], zt = r_cubzte[k];
          const double drdx = (ys * zt - zs * yt);
          const double drdy = -(xs * zt - zs * xt);
          const double drdz = (xs * yt - ys * xt);
          const double dsdx = -(yr * zt - zr * yt);
          const double dsdy = (xr * zt - zr * xt);
          const double dsdz = -(xr * yt - yr * xt);
          const double dtdx = (yr * zs - zr * ys);
          const double dtdy = -(xr * zs - zr * xs);
          const double dtdz = (xr * ys - yr * xs);
          const double W = s_cubW[i] * s_cubW[j] * s_cubW[k];
          const double Un = r_U[k];
          const double Vn = r_V[k];
          const double Wn = r_W[k];
          const int id = element * 1331 + k * 11 * 11 + j * 11 + i;
          result[id + 0 * cubatureOffset] = W * (Un * drdx + Vn * drdy + Wn * drdz);
          result[id + 1 * cubatureOffset] = W * (Un * dsdx + Vn * dsdy + Wn * dsdz);
          result[id + 2 * cubatureOffset] = W * (Un * dtdx + Vn * dtdy + Wn * dtdz);
        }
      }
      ;
    }
  }
}

