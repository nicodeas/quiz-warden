export const renderAnswer = (data, type) => {
  const answerEl = document.getElementById("answer-container");
  // Checks the type of the question to determine how the answer is rendered
  if (type === "IMAGE") {
    // Directly embed base64 image data
    answerEl.innerHTML = `<img src="data:image/png;base64, ${data.answer}" />`;
  } else {
    answerEl.innerHTML = `<p>The answer is: <span id="correct-answer"></span></p>`;
    // If text, decode the base64 to plaintext
    document.getElementById("correct-answer").innerText = atob(data.answer);
  }
};
