let btn = document.getElementById('btn1');
let slider = document.getElementById('myRange');

slider.oninput = function(){
	btn.innerText = slider.value;
}

function message() {
	swal("Good job!", "You clicked the button!", "success");
}