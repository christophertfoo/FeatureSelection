require('rpart')

createFolds <- function(data, nfolds=10) {
  folds <- list()
  indexes <- split(sample(1:nrow(data)), 1:nfolds)
  for(i in length(indexes)) {
    folds[[i]] <- data[test[[i]],] 
  }
  return(folds)
}

tenFold <- function(folds, features) {
  
}

car <- read.csv('car.csv')
mushroom <- read.csv('mushroom.csv')