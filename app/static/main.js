var video = document.getElementById('video');
// getUserMedia()でカメラ映像の取得
var media = navigator.mediaDevices.getUserMedia({ video: true });
//リアルタイム再生（ストリーミング）を行うためにビデオタグに流し込む
media.then((stream) => {
    video.srcObject = stream;
});

var canvas = document.getElementById('canvas');
canvas.setAttribute('width', video.width);
canvas.setAttribute('height', video.height);

video.addEventListener(
    'timeupdate',
    function () {
        var context = canvas.getContext('2d');
        context.drawImage(video, 0, 0, video.width, video.height);
    },
    true
);

// スペースキー押下時にキャプチャ取得を実行するリスナーを設定
document.addEventListener('keydown', (event) => {
    var keyName = event.key;
    if (keyName === 'a') {
        console.log(`keydown: SpaceKey`);
        context = canvas.getContext('2d');
        // 取得したbase64データのヘッドを取り除く
        var img_base64 = canvas.toDataURL('image/jpeg').replace(/^.*,/, '')
        captureImga(img_base64);
    }
    if (keyName === 'b') {
        console.log(`keydown: SpaceKey`);
        context = canvas.getContext('2d');
        // 取得したbase64データのヘッドを取り除く
        var img_base64 = canvas.toDataURL('image/jpeg').replace(/^.*,/, '')
        captureImgb(img_base64);
    }
});

var xhr = new XMLHttpRequest();

// キャプチャ画像データ(base64)をPOST
function captureImga(img_base64) {
    const body = new FormData();
    body.append('img', img_base64);
    xhr.open('POST', 'http://localhost:8000/capture_imga', true);
    xhr.onload = () => {
        console.log(xhr.responseText)
    };
    xhr.send(body);
}

function captureImgb(img_base64) {
    const body = new FormData();
    body.append('img', img_base64);
    xhr.open('POST', 'http://localhost:8000/capture_imgb', true);
    xhr.onload = () => {
        console.log(xhr.responseText)
    };
    xhr.send(body);
}

