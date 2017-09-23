# 프로젝트 환경 설정 방법
.( 걍 받고 실행시키면 됩니다. 하단의 환경설정은 bongbong 홈피에서 막 받은 프로젝트 파일 & 라이브러리를 쓸때만 해당)
 
1. bongbong에서 주는 DX winzip파일 압축을 풀고 Include와 Lib폴더 두개만 프로젝트 폴더에 넣는다.
2. 프로젝트 속성에 들어가서 링커->고급-> 이미지에 안전한 예외 처리기 포함을 "아니오" 로 설정
3. VC++ 디렉터리->포함(Include) 디렉터리를 1번의 Include폴더로 설정
4. VC++ 디렉터리->라이브러리(Library) 디렉터리를 1번의 Lib폴더로 설정
5. 포함/라이브러리 디렉터리를 보면 대충 
C:\Users\user\Documents\GitHub\proj3_VirtualBilliard\Include;$(IncludePath)
이렇게 되어있을 것이다. 순서를 바꿔준다

$(IncludePath);C:\Users\user\Documents\GitHub\proj3_VirtualBilliard\Include
$(LibraryPath);C:\Users\user\Documents\GitHub\proj3_VirtualBilliard\Lib
이렇게

6.d3dUtility.h 가장 상단에 #pragma warning(disable:2026) 추가
7.d3dUtility.h 가장 하단에 const float INFINITY = FLT_MAX; 를 const float INF = FLT_MAX; 로 수정
8.d3dUtility.cpp에서 INFINITY를 모두 INF로 수정

# 코드 컨벤션

1. 괄호  
 main()  
 {
 
 }
2. 변수이름  
 hello_world (NOT helloWorld)
3. 주석은 변수나 주요 구현 부분에 꼬박꼬박 달아주세요.
