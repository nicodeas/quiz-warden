export const renderState = async (isQuiz, state) => {
  const { currentQuestion, maxQuestions, activeQBs } = state;

  const pythonBankStatusElement = document.getElementById("python-bank-status");
  const cBankStatusElement = document.getElementById("c-bank-status");

  const pythonBankStatus = activeQBs.includes("Python");
  const cBankStatus = activeQBs.includes("C");

  pythonBankStatusElement.innerText = pythonBankStatus ? "Online" : "Offline";
  pythonBankStatusElement.style.color = pythonBankStatus ? "green" : "red";
  cBankStatusElement.innerText = cBankStatus ? "Online" : "Offline";
  cBankStatusElement.style.color = cBankStatus ? "green" : "red";

  if (isQuiz) {
    document.getElementById("total-marks").innerText = state.totalMarks;

    const currentQuestionElement = document.getElementById("current-question");
    const maxQuestionsElement = document.getElementById("max-questions");
    currentQuestionElement.innerText = currentQuestion;
    maxQuestionsElement.innerText = maxQuestions;

    // conditionally remove navigation buttons
    const nextButton = document.getElementById("next-button");
    const backButton = document.getElementById("back-button");

    if (currentQuestion == state.maxQuestions) {
      nextButton.style.display = "none";
    }

    if (currentQuestion == 1) {
      backButton.style.display = "none";
    }
  } else {
    const usernameElement = document.getElementById("username");
    usernameElement.innerText = state.username;
  }
};
