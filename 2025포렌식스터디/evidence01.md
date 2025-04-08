evidence 01 라이트업
=============


### 1. What is the name of Ann’s IM buddy? 

![image](https://github.com/user-attachments/assets/46b57669-71a2-4599-a91d-5c3188cda32c)

network miner를 열어 message 창을 체크해보면 ann의 채팅 상대의 닉네임이 **Sec559user1** 이라는 것을 알 수 있다.


### 2.  What was the first comment in the captured IM conversation? 

![image](https://github.com/user-attachments/assets/70dc1b79-2982-4039-9556-efb13e633736)

같은 창에서 첫 번째 채팅의 내용도 확인할 수 있다. 

**Here's the secret recipe... I just downloaded it from the file server. Just copy to a thumb drive and you're good to go &gt;:-)** 라는 내용이다. 


### 3. What is the name of the file Ann transferred?

![image](https://github.com/user-attachments/assets/bed4e4e2-ac5d-4b3c-9c96-a55450566703)

Files 칸을 체크해보면 Ann이 보낸 파일을 알 수 있다. **recipe.docx** 파일이다.

### 4. What is the magic number of the file you want to extract (first four bytes)? 

![image](https://github.com/user-attachments/assets/3095b826-4296-45bf-94e4-4ade9e41a6dd)

files 칸에서 recipe.docx를 더블클릭하면 파일의 정보를 확인할 수 있다. 첫 4바이트는 **50 4B 03 04** 이다. 
이는 docx 파일의 파일 시그니처이기도 하다.

### 5. What was the MD5sum of the file?

![image](https://github.com/user-attachments/assets/a1056e10-1995-4ec1-9311-c9f73efbf65b)

마찬가지로 여기서 recipe.docx의 MD5 해시값도 확인할 수 있다.

**8350582774e1d4dbe1d61d64c89e0ea1** 이다.

### 6. What is the secret recipe? 

![image](https://github.com/user-attachments/assets/c2dfef4c-f1fb-49d5-942b-5f481d1e5f37)

open file 기능을 사용하여 recipe.docx 파일을 열면 이렇게 word를 사용하여 열린다. 파일의 내용은 위와 같다.
