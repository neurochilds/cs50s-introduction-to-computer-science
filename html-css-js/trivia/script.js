const mcqButtons = document.querySelectorAll("button.mcq")
const answersMCQ = {
    "god-of-thunder": "jupiter",
    "god-of-dead": "hades",
    "god-of-messages": "hermes"
}
const answersTyped = {
    "god-of-love": "aphrodite",
    "god-of-sea": "poseidon",
    "hero-twelve-labours": "heracles"
}

mcqButtons.forEach(function(button){
    button.addEventListener('click', () => {
        let question = button.parentElement.dataset.question
        let currentButtons = button.parentElement.querySelectorAll('button')
        let result = button.parentElement.querySelector('p.result')

        currentButtons.forEach(function(button){
            button.classList.remove('correct', 'incorrect')
            result.textContent = ''
        })

        if (answersMCQ[question] == button.value){
            button.classList.add('correct')
            result.textContent = 'Correct!'
        } else {
            button.classList.add('incorrect')
            result.textContent = 'Incorrect'
        }
    })
})

function submitAnswer(button){
    let question = button.parentElement.dataset.question
    let inputBox = button.parentElement.querySelector('input')
    let answer = inputBox.value
    let result = button.parentElement.querySelector('p.result')

    if (answersTyped[question] == answer.toLowerCase()){
        inputBox.classList.remove('incorrect')
        inputBox.classList.add('correct')
        result.textContent = 'Correct!'
    } else {
        inputBox.classList.add('incorrect')
        result.textContent = 'Incorrect'
    }
}