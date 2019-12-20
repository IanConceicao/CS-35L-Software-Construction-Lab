#!usr/bin/env python3
import os
import subprocess
import zlib

def topo_order_commits():
    class CommitNode:
        def __init__(self, commit_hash):
            """
            :type commit_hash: str
            """
            self.commit_hash = commit_hash
            self.parents = set()
            self.children = set()

    ####   FIND RELATIVE PATH OF .GIT ####
    found = False
    path = ""
    atRoot = False

    while not found and not atRoot:
        for root, dirs, files in os.walk("."):
            for dirname in dirs:
                if dirname == ".git":
                    found = True
                    path += ".git"
            if not found:
                path += "../"
            if os.getcwd() == "/" or os.getcwd() == "/w/home.18/cs/ugrad/conceica":
                atRoot = True
            if not atRoot and not found:
                os.chdir("..")
            break #Don't traverse subdirs

    if not found:
        print("Not inside a Git repository")
        exit(1)

    ###### END OF FINDING .GIT PATH ######

    ###### LIST OF LOCAL BRANCH NAMES ######

    #go to the folder
    os.chdir("./.git/refs/heads")
    cmd = subprocess.Popen('ls', stdout=subprocess.PIPE)
    branchesStr, cmd_err = cmd.communicate()
    branchesStr = branchesStr.decode("utf-8")
    branches = branchesStr.splitlines()
    branchesHash = []
    branchName = dict()

    for branch in branches:
        fd = os.open(branch,os.O_RDWR)
        hashed = os.read(fd,40).decode("utf-8")
        if(not hashed in branchesHash):
            branchesHash.append(hashed)
            branchName[hashed] = [branch]
        else:
            branchName[hashed].append(branch)
            branchName[hashed].sort()
        os.close(fd)

    #Now we have all have all branch commits in there, branches

    ###### END OF LIST OF LOCAL BRANCH NAMES ####

    ####### BUILD THE COMMIT GRAPH #########


    #Returns a list of parent hashes for a given hash
    def getParents(hash):
        os.chdir("../../objects") #go into .git/objects
        folder = hash[0:2]
        os.chdir(folder)
        fd = os.open(hash[2:40],os.O_RDONLY)
        statInfo = os.stat(hash[2:40])
        fileSize = statInfo.st_size
        compressed = os.read(fd,fileSize)

        plainText = zlib.decompress(compressed).decode("utf-8")
        plainText = plainText[0:plainText.find("author")]

        parentHashes = set()
        while(plainText.find("parent") != -1):
            index = plainText.find("parent") + 7
            parentHashes.add(plainText[index:index+40])
            plainText = plainText[index+40:]
        os.close(fd)
        return parentHashes

    root_commits = []
    visitedNodes = []
    def setRelationship(node):
        visitedNodes.append(node)
        parentHashes = getParents(node.commit_hash)
        if(len(parentHashes) == 0):
            root_commits.append(node)
        for hash in parentHashes:
            parentVisited = False
            for otherNode in visitedNodes:
                if(otherNode.commit_hash == hash):   #if parent node already
                    otherNode.children.add(node) #made
                    node.parents.add(otherNode)
                    parentVisited = True
            if not parentVisited:
                newNode = CommitNode(hash)
                node.parents.add(newNode)
                newNode.children.add(node)
                setRelationship(newNode)

    for commitBranch in branchesHash:
        branchNode = CommitNode(commitBranch)
        setRelationship(branchNode)

    ###### FINISHED BUILDING COMMIT GRAPH ######

    #### CREATE TOPLOGICAL GRAPH #####

    topological = list()
    nodeStack = []

    def visit(node):
        nodeStack.append(node)
        for child in node.children:
            visit(child)
        node = nodeStack.pop()
        if not( node in topological):
            topological.append(node)

    for root in root_commits:
        visit(root)


    #### FINISHED CREATING TOPOLOGICAL GRAPH ####


    #### OUTPUT TOPLOGOCIAL GRAPH #####
    #Remember we have branchName dict; branchName[hash] = name

    def isParent(child, potentialParent):
        for parent in child.parents:
            if(potentialParent.commit_hash == parent.commit_hash):
                return True
        return False

    for index, node in enumerate(topological):

        #If previous is not the child
        if(index > 0 and not isParent(topological[index-1],node)):
            print("=",end="")
            for child in node.children:
                print(child.commit_hash, end = " ")

        if(node.commit_hash in branchName):
            print(node.commit_hash,end="")
            for branch in branchName[node.commit_hash]:
                print(" " + branch,end="")
            print()
        else:
            print(node.commit_hash)
        #If next up is not the parent
        if(index < len(topological) -1 and not isParent(node, topological[index+1])):
            first = True
            for parent in node.parents:
                if first:
                    print(parent.commit_hash,end="")
                    first = False
                else:
                    print(" " + parent.commit_hash, end = "")
            print("=")
            print()


    #### FINISHED OUTPUT #####

if __name__ == '__main__':
    topo_order_commits()
