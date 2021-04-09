# Cooperative-Scheduler
# Unit tests:
**Test for the ready queue**
                                                        
This part has four tasks. Here, no need to call “Rerun” because it is only the ready queue test. The following table describes their definition: \
- If all tasks have different priorities, so the one with the highest priority level (least priority number) executes first. 
![image](https://user-images.githubusercontent.com/45397911/114049532-cd77f900-988b-11eb-8d1f-5a5ecc354ad2.png)

###### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114049574-d8328e00-988b-11eb-9554-35028c147234.png)
###### The Scheduler output:
 ![image](https://user-images.githubusercontent.com/43725885/114219942-a3960380-996b-11eb-84b1-350bde171506.png)
- If two tasks have the same priority level, so anyone of them executes first.
![image](https://user-images.githubusercontent.com/45397911/114049840-1334c180-988c-11eb-87ca-f2caf5d4b99e.png)
###### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114049903-1fb91a00-988c-11eb-88bc-ebbba8f653e3.png)
###### The Scheduler output:
![image](https://user-images.githubusercontent.com/43725885/114223819-8c0d4980-9970-11eb-887e-a2b6201036fc.png)
- If two tasks have the same priority level, so anyone of them executes first.
![image](https://user-images.githubusercontent.com/45397911/114050057-3e1f1580-988c-11eb-88ae-6d7985a9a208.png)
###### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114050162-51ca7c00-988c-11eb-8c40-78afbb1ed6a3.png)
###### The Scheduler output:
![image](https://user-images.githubusercontent.com/43725885/114225815-15be1680-9973-11eb-98f7-b0d0a3b1c46b.png)
-	If two tasks have the same priority level, so anyone of them executes first.
![image](https://user-images.githubusercontent.com/45397911/114050319-732b6800-988c-11eb-99c7-e201065bf491.png)
###### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114050353-7c1c3980-988c-11eb-9241-bf4a2bd36e38.png)
###### The Scheduler output:
![image](https://user-images.githubusercontent.com/43725885/114226077-69306480-9973-11eb-9778-ff9653241c83.png)
- If all tasks have the same priority level, so anyone of them executes first.
![image](https://user-images.githubusercontent.com/45397911/114050470-95bd8100-988c-11eb-8c1d-cc95ffa7a94a.png)
###### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114050507-9eae5280-988c-11eb-8114-1b45ddbf4d27.png)
###### The Scheduler output:
![image](https://user-images.githubusercontent.com/43725885/114226866-7e59c300-9974-11eb-8141-d4b8c619dbd6.png)
**Test for the delayed queue**
- If all of them have different priorities:
![image](https://user-images.githubusercontent.com/45397911/114051010-08c6f780-988d-11eb-90c8-3d0669a3762a.png)
###### Expected output:
![image](https://user-images.githubusercontent.com/45397911/114051058-141a2300-988d-11eb-8862-5e49c5047764.png)
###### The Scheduler output:
![image](https://user-images.githubusercontent.com/43725885/114228665-15277f00-9977-11eb-9fc1-14c50a4281f6.png)

