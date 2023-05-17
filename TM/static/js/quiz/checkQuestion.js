// Makes a POST request with the index of the chosen answer
//TODO: work with QB
export const checkQuestion = async (questionAnswer) => {
    const questionResultText = document.getElementById("question-result");
    const questionAttemptsText = document.getElementById("question-attempts");
    const questionMarkText = document.getElementById("question-attempts");
  
    const data = { answer: questionAnswer, number: currentQuestionIndex };
    fetch("/api/question", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(data),
    })
      .then((response) => response.json())
      .then((data) => {
        const result = data.correct ? "Correct!" : "Incorrect!";
        questionResultText.innerHTML = result;
  
        // The question has been answered correctly
        if (data.mark) {
          checkButton.disabled = true;
          questionAttemptsText.innerHTML = "";
          questionMarkText.innerHTML = "Your mark is: " + data.mark;
        } else {
          questionAttemptsText.innerHTML =
            "You have " + data.attempts + " Attempts!";
        }
      })
      .catch((error) => {
        console.log(error);
        alert("An error occurred checking the answer!");
      });
  };
  