
kernel void vector_add_shader(
    device const float *A[[buffer(0)]],
    device const float *B[[buffer(1)]],
    device float       *C[[buffer(2)]],
    constant uint      &N[[buffer(3)]], 
    uint2 gid [[thread_position_in_grid]]
) {
    uint i = gid.x;

    if (i < N) {
        C[i] = A[i] + B[i];
    }
}