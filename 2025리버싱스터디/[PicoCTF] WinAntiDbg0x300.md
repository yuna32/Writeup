[PicoCTF] WinAntiDbg0x300
========================

x32dbg로 열어봤는데 구조를 모르겠어서 ida에서 디컴파일해봤다.

```c
void __usercall start(int a1@<eax>, int a2@<edx>, int a3@<ecx>)
{
  int *v3; // esi
  int *v4; // edi
  unsigned int v5; // ebp
  char v6; // al
  bool v7; // cf
  int v8; // ebx
  int v9; // ebx
  int v10; // ett
  unsigned int v11; // eax
  bool v12; // cf
  int v13; // ebx
  int v14; // ebx
  int v15; // ett
  bool v16; // zf
  int v17; // ebx
  int v18; // ett
  unsigned int v19; // eax
  unsigned int v20; // eax
  int v21; // eax
  bool v22; // cf
  int v23; // ebx
  int v24; // ebx
  int v25; // ett
  BOOL v26; // ecx
  bool v27; // cf
  int v28; // ebx
  int v29; // ett
  int v30; // ecx
  int v31; // ecx
  bool v32; // cf
  int v33; // ebx
  int v34; // ebx
  int v35; // ett
  bool v36; // zf
  int v37; // ebx
  int v38; // ett
  unsigned int v39; // ecx
  char *v40; // edx
  char v41; // al
  int v42; // eax
  bool v43; // cc
  int *v44; // esi
  int *v45; // edi
  int v46; // ecx
  char v47; // al
  unsigned __int8 v48; // al
  int v49; // eax
  int v50; // eax
  char v51; // t0
  unsigned __int8 v52; // bl
  char *v53; // edi
  int *v54; // ebx
  int v55; // ebp
  char v56; // al
  char *v57; // ecx
  unsigned __int8 v58; // al
  int v59; // eax
  _WORD *v60; // edi
  int *i; // ebx
  int v62; // eax
  int v63; // eax
  int v64; // eax
  char v65; // tt
  void (__cdecl *v66)(int *, int, int, int *); // ebp
  int *v67; // edi
  int v68[4]; // [esp+1Ch] [ebp-90h] BYREF
  int v69; // [esp+2Ch] [ebp-80h] BYREF
  int v70; // [esp+30h] [ebp-7Ch] BYREF
  char *v71; // [esp+34h] [ebp-78h]
  int *v72; // [esp+38h] [ebp-74h]
  int v73; // [esp+3Ch] [ebp-70h]
  int v74; // [esp+40h] [ebp-6Ch]
  int v75; // [esp+44h] [ebp-68h]
  int v76; // [esp+48h] [ebp-64h]
  int *v77; // [esp+4Ch] [ebp-60h]
  int v78; // [esp+50h] [ebp-5Ch]
  int v79; // [esp+54h] [ebp-58h]
  int v80; // [esp+58h] [ebp-54h]
  int v81; // [esp+5Ch] [ebp-50h] BYREF

  v77 = &v81;
  v75 = a3;
  v74 = a2;
  v73 = a1;
  v3 = dword_416000;
  v4 = &dword_416000[-21504];
  v72 = &dword_416000[-21504];
  v5 = -1;

 while ( 1 )
  {
    v9 = *v3;
    v7 = (unsigned int)v3++ < 0xFFFFFFFC;
    v10 = v7 + v9;
    v7 = __CFADD__(v7, v9) | __CFADD__(v9, v10);
    v8 = v9 + v10;
    do
    {
      if ( v7 )
      {
        v6 = *(_BYTE *)v3;
        v3 = (int *)((char *)v3 + 1);
        *(_BYTE *)v4 = v6;
        v4 = (int *)((char *)v4 + 1);
      }
      else
      {
        v11 = 1;
        while ( 1 )
        {
          v12 = __CFADD__(v8, v8);
          v13 = 2 * v8;
          if ( !v13 )
          {
            v14 = *v3;
            v7 = (unsigned int)v3++ < 0xFFFFFFFC;
            v15 = v7 + v14;
            v12 = __CFADD__(v7, v14) | __CFADD__(v14, v15);
            v13 = v14 + v15;
          }
          v11 += v12 + v11;
          v7 = __CFADD__(v13, v13);
          v16 = 2 * v13 == 0;
          v8 = 2 * v13;
          if ( v7 )
          {
            if ( !v16 )
              break;
            v17 = *v3;
            v7 = (unsigned int)v3++ < 0xFFFFFFFC;
            v18 = v7 + v17;
            v7 = __CFADD__(v7, v17) | __CFADD__(v17, v18);
            v8 = v17 + v18;
            if ( v7 )
              break;
          }
        }
        v7 = v11 < 3;
        v19 = v11 - 3;
        if ( !v7 )
        {
          v20 = v19 << 8;
          LOBYTE(v20) = *(_BYTE *)v3;
          v3 = (int *)((char *)v3 + 1);
          v21 = ~v20;
          if ( !v21 )
          {
           v44 = v72;

(....)

}

```

라고 쭉 이어지는 엄청 긴 코드가 나오는데 길이나 구성을 봐서 인코딩/디코딩에 사용되는 건가 짐작될 뿐이고 어떻게 풀어야할지 방향을 모르겠다... 
