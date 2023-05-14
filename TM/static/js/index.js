const startQuiz = async () => {
  try {
    await fetch("http://localhost:8000/api/generatequiz");
    window.location.href = "http://localhost:8000/quiz";
  } catch (err) {
    console.log(err);
  }
};
