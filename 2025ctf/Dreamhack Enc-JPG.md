dreamhack Enc-JPG writeup
==========


<img width="747" height="182" alt="image" src="https://github.com/user-attachments/assets/4100156f-3590-41b1-9bed-6d5c1687f38d" />

enc 파일의 경우 exe 파일의 헤더 시그니처인 MZ가 확인됨    

<img width="778" height="131" alt="image" src="https://github.com/user-attachments/assets/fbc4eefb-73f5-4e22-a79f-a8f64accd558" />

flag.jpg의 경우 jpg 파일이지만 알맞은 헤더 시그니처가 들어가있지 않음    




<img width="362" height="140" alt="image" src="https://github.com/user-attachments/assets/7b991d51-b68a-4fcb-bf40-173be03c5565" />

enc의 확장자를 exe로 바꾸어 실행하면 이렇게 flag의 이미지가 바뀜    



<img width="1121" height="155" alt="image" src="https://github.com/user-attachments/assets/42661b6c-cff8-4bfd-bc89-ee97d6e04187" />


이미지가 잘려있으므로 다시 헥스에디터에서 확인    
jpg의 푸터 시그니처 FF D9를 검색하면 두 개의 검색 결과가 나온다. 여기서 첫 번째 푸터 시그니처를 제거    

<img width="367" height="102" alt="image" src="https://github.com/user-attachments/assets/8f60832d-249c-4a01-b157-63595fbaf0b1" />

그러면 이렇게 플래그의 일부를 확인 가능하다.    

<img width="805" height="132" alt="image" src="https://github.com/user-attachments/assets/5f1e5c0a-6466-4382-b5af-b29e6ad9d744" />


두 번째 푸터 시그니처 위치에는 find the string "_ENc_ECrypt" 라는 문자열이 있다. 일단 킵해두고    


<img width="716" height="87" alt="image" src="https://github.com/user-attachments/assets/48a40f72-346d-45b4-b197-a6ce45dbfe21" />

마지막 남은 부분은 } 형태일 것이므로 }를 서치한다. 이 중 플래그의 일부분처럼 보이는 문자열 _yo} 를 확인 가능하다.   


얻은 문자열들을 합치면 DH{How_ENc_ECrypt_yo} 가 된다. 


