let question = "";
choices = [];
let currentQuestionIndex = 1;
type = "";

const getQuestion = async (previousQuestionIndex) => {
  document.getElementById("question-result").innerHTML = "";
  const language = document.getElementById("language");
  language.innerHTML = "";

  fetch("/api/question?number=" + currentQuestionIndex, {
    method: "GET",
    headers: {
      "Content-Type": "application/json",
    },
  })
    .then((response) => {
      if (!response.ok) {
        currentQuestionIndex = previousQuestionIndex;
        throw new Error("An error occurred fetching the question!");
      }
      return response.json();
    })
    .then((data) => {
      question = data.question;
      if (data.type == "CHOICE") {
        choices = data.choices;
        type = data.type;
      } else if (data.type == "CODE") {
        type = data.type;
        language.innerHTML = " using " + data.language;
      }
      updateQuiz();
    });
};

const updateQuiz = () => {
  const answerContainer = document.getElementById("quiz-choices");
  // Reset the container to blank
  answerContainer.innerHTML = "";
  document.getElementById("quiz-question").innerHTML = question;

  if (type == "CHOICE") {
    // create multichoice fields
    choices.forEach((choice, idx) => {
      const radio = document.createElement("input");
      radio.type = "radio";
      radio.name = "option";
      radio.value = choice;
      radio.id = `choice-${idx}`;
      const label = document.createElement("label");
      label.innerHTML = choice;
      label.setAttribute("for", `choice-${idx}`);

      const choiceContainer = document.createElement("div");
      choiceContainer.appendChild(radio);
      choiceContainer.appendChild(label);
      answerContainer.appendChild(choiceContainer);
    });
  } else if (type == "CODE") {
    const textarea = document.createElement("textarea");
    textarea.id = "code-answer";
    answerContainer.append(textarea);
  }
};
// Makes a POST request with the index of the chosen answer
const checkQuestion = async (questionAnswer) => {
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

getQuestion();

/*     EVENT LISTENERS     */
const checkButton = document.getElementById("check-button");

checkButton.addEventListener("click", function (e) {
  e.preventDefault();
  if (type == "CHOICE") {
    const options = document.getElementsByName("option");
    let checkedQuestionIndex = -1;
    for (let i = 0; i < options.length; i++) {
      if (options[i].checked) {
        checkedQuestionIndex = i;
        break;
      }
    }
    if (checkedQuestionIndex === -1) {
      alert("Please select an answer!");
      return;
    }
    checkQuestion(checkedQuestionIndex);
  } else if (type == "CODE") {
    const answer = document.getElementById("code-answer").value;

    checkQuestion(answer);
  }
});

document.getElementById("next-button").addEventListener("click", function (e) {
  e.preventDefault();
  const questionAttemptsText = document.getElementById("question-attempts");
  questionAttemptsText.innerHTML = "";
  checkButton.disabled = false;

  let previousQuestionIndex = currentQuestionIndex;
  currentQuestionIndex++;
  getQuestion(previousQuestionIndex);
});

document.getElementById("back-button").addEventListener("click", function (e) {
  e.preventDefault();
  const questionAttemptsText = document.getElementById("question-attempts");
  questionAttemptsText.innerHTML = "";
  checkButton.disabled = false;

  let previousQuestionIndex = currentQuestionIndex;
  currentQuestionIndex--;
  getQuestion(previousQuestionIndex);
});
