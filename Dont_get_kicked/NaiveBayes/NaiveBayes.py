import sys ; import os
import pandas as pd


assert len(sys.argv) == 3, "The number of input files should be 3. ProgramName, train file & test file"
training_file = os.path.abspath(sys.argv[1])
test_file = os.path.abspath(sys.argv[2])

def predict( lines, prob, prob_total_plus, prob_total_minus):
    # Now classifying the rows in training set based on the
    # conditional probability using the Bayes' theoram
    true_positive = 0
    true_negative = 0
    false_positive = 0
    false_negative = 0
    decision = []
    for line in lines:
        # Classify this line using Bayes' theoram
        output = line['class']
        positive_prob = prob_total_plus
        negative_prob = prob_total_minus
        for key, val in line.iteritems():
            if key == 'class':
                continue
            if ((key in prob.keys()) is False):
                continue
            if ((val in prob[key].keys()) is False):
                continue
            
            cur_pos_prob = prob[key][val][0]
            cur_neg_prob = prob[key][val][1]
            positive_prob *= cur_pos_prob
            negative_prob *= cur_neg_prob
    
        if ( positive_prob >= negative_prob ):
            decision.append(1)
            if ( output == '1'):
                true_positive += 1
            else:
                false_positive += 1
        if ( negative_prob > positive_prob ):
            decision.append(0)
            if ( output == '1'):
                false_negative += 1
            else:
                true_negative += 1
            
    print str(true_positive) + ' ' + str(false_negative) + ' ' + str(false_positive) + ' ' + str(true_negative)
    accuracy = float(true_positive + true_negative)/float(true_positive + true_negative + false_positive + false_negative)
    print accuracy
    return decision

def store( training_file):
    lines = []
    attr_values = {}
    total_plus = 0
    total_minus = 0
    for line in open(training_file):
        entry = {}
        line = line.split(' ')
        tmp = line.pop(0)
        if tmp == '1':
            total_plus += 1
        else:
            total_minus += 1
        entry['class'] = tmp
        for kv in line:
            kv = kv.split(':')
            entry[int(kv[0])] = int(kv[1])
            if ( (int(kv[0]) in (attr_values.keys())) is False ):
                attr_values[int(kv[0])] = {}
                attr_values[int(kv[0])][int(kv[1])] = {}
                attr_values[int(kv[0])][int(kv[1])]['1'] = 0
                attr_values[int(kv[0])][int(kv[1])]['0'] = 0
                if tmp == '1':
                    attr_values[int(kv[0])][int(kv[1])]['1'] = 1
                else:
                    attr_values[int(kv[0])][int(kv[1])]['0'] = 1
            else:
                if ( (int(kv[1]) in (attr_values[(int(kv[0]))].keys())) is False ):
                    attr_values[int(kv[0])][int(kv[1])] = {}
                    attr_values[int(kv[0])][int(kv[1])]['1'] = 0
                    attr_values[int(kv[0])][int(kv[1])]['0'] = 0
                    if tmp == '1':
                        attr_values[int(kv[0])][int(kv[1])]['1'] += 1
                    else:
                        attr_values[int(kv[0])][int(kv[1])]['0'] += 1
                else:
                    if tmp == '1':
                        attr_values[int(kv[0])][int(kv[1])]['1'] += 1
                    else:
                        attr_values[int(kv[0])][int(kv[1])]['0'] += 1
        
        lines.append(entry)
    
    return lines, attr_values, total_plus, total_minus

def generate_prob_all ( attr_values, total_plus, total_minus):
    # The combined probablity of all values for all attributes
    prob = {}
    for attr, attr_val_hash in attr_values.iteritems():
        prob[attr] = {}
        for attr_val, attr_val_occur in attr_val_hash.iteritems():
            p, q = attr_val_occur['1'], attr_val_occur['0']
            prob[attr][attr_val] = ( ( p / float(total_plus) ), ( q / float(total_minus) ) )
    
    return prob

# Work on training file
training_lines, training_attr_values, train_total_plus, train_total_minus = store(training_file)
training_prob_total_plus    = train_total_plus / float(len(training_lines))
training_prob_total_minus   = train_total_minus / float(len(training_lines))

prob_training = generate_prob_all( training_attr_values, train_total_plus, train_total_minus)
predict( training_lines, prob_training, training_prob_total_plus, training_prob_total_minus)

# Work on test file
test_lines, test_attr_values, test_total_plus, test_total_minus = store(test_file)
# Giving equal likelihood probability for yes and no, insteadof skewed prob from training data
decision = predict( test_lines, prob_training, 0.5, 0.5)

# Print the submission file
get_refid = pd.read_csv('../kNN/orig_test.csv')
predictions = pd.Series(decision)
print predictions.value_counts()
submission = pd.DataFrame({'RefId': get_refid.RefId, 'IsBadBuy': predictions})
cols = submission.columns.tolist()
cols = cols[-1:] + cols[:-1]
submission = submission[cols]
submission.to_csv('submission_bayes.csv', index=False)
