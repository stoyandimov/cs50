from nltk.tokenize import sent_tokenize


def extractSubsting(s, n):
    result = []
    for i in range(len(s) - n + 1):
        sub = s[i:i+n]
        result.append(sub)
        # print(s + ":" + sub)
    return result


def lines(a, b):
    """Return lines in both a and b"""
    aLines = set(a.split('\n'))
    bLines = set(b.split('\n'))
    # 'set' here ensures lists are unique
    return list(set(aLines & bLines))


def sentences(a, b):
    """Return sentences in both a and b"""
    aSents = set(sent_tokenize(a))
    bSents = set(sent_tokenize(b))
    return list(set(aSents & bSents))


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    aSubs = set(extractSubsting(a, n))
    bSubs = set(extractSubsting(b, n))
    return list(aSubs & bSubs)

