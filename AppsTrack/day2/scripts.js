console.log('hi');

//const sleep=new Promise((resolve,reject) => {
//
//    try{
//        setTimeout(()=>{
//        console.log("delayed output");
//        resolve("done");
//        },2000);
//    }
//    catch{
//        reject("Somthing went wrong");
//    }
//});
//
//sleep.then(data => console.log(data)).catch(error => console.error('Eroor:',error));

//const promiseArray = []
//
//for (let i=0;i<10;i++){
//    const newPromise = new Promise((resolve,reject)=>{
//        console.log(i);
//        resolve(i);
//    });
//    newPromise.then((number)=>console.log('done',number));
//    promiseArray.push(newPromise);
//}
//
//Promise.all(promiseArray).then(() => console.log('finish'));
//console.log("Bye Bye")

fetch('https://api.github.com/users/octocat')
    .then(result=>{
        const jsonPr=result.json();
        jsonPr.then(data=>{
            console.log(data);
            console.log(data.name);
        });
    });